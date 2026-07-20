#!/usr/bin/env python3
"""Render the CV page(s) of the MkDocs site to a clean, one-file PDF.

This is a standalone, on-demand tool. It is intentionally NOT part of the
site build or `mkdocs gh-deploy` — the GitHub Pages site does not need a PDF to
deploy, and its heavy dependency (a headless Chromium via Playwright) is kept
out of the core `requirements.txt`. It only ever touches the CV page
(`index` / `ko/index`), never the rest of the study wiki.

Source of truth stays single: this reads the same `docs/index.md` /
`docs/index.ko.md` that the live site renders, so the PDF can never drift from
the site.

Usage:
    python -m venv .venv-cv && source .venv-cv/bin/activate
    pip install -r requirements-cv.txt
    playwright install chromium          # one-time, downloads Chromium
    python scripts/build_cv_pdf.py               # -> docs/cv.pdf, docs/cv.ko.pdf
    python scripts/build_cv_pdf.py --lang en     # English only
    python scripts/build_cv_pdf.py --out dist    # write into dist/ instead

The PDFs land in docs/ so `mkdocs build` / `gh-deploy` publishes them alongside
the site, letting the CV page link to them for download.
"""

from __future__ import annotations

import argparse
import contextlib
import functools
import http.server
import socket
import socketserver
import subprocess
import sys
import threading
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
SITE_DIR = REPO_ROOT / "site"

# CV page path within the built site, per language. The i18n plugin builds the
# default locale (en) at the root and other locales under their code.
LANG_PATHS = {"en": "index.html", "ko": "ko/index.html"}

# Injected before printing: strip the Material chrome (header, nav sidebars,
# footer, search) so the PDF is just the CV content, and give it print-friendly
# width/margins. The site itself is untouched — this lives only in the headless
# render.
PRINT_CSS = """
.md-header, .md-tabs, .md-sidebar, .md-footer,
.md-content__button, .md-search, [data-md-component="announce"] { display: none !important; }
.md-main__inner { margin: 0 !important; }
.md-content { margin: 0 !important; }
.md-content__inner { padding: 0 1.2rem !important; margin: 0 !important; }
.md-grid { max-width: none !important; }
.md-typeset { font-size: 0.72rem; }
.md-typeset h1 { margin-top: 0; }
.md-typeset img { vertical-align: middle; }
a { text-decoration: none !important; }
"""


def _free_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(("127.0.0.1", 0))
        return s.getsockname()[1]


@contextlib.contextmanager
def _serve(directory: Path):
    """Serve `directory` over localhost so remote badge images load correctly.

    Loading via http:// (not file://) keeps relative asset URLs and the
    shields.io badge requests working exactly as they do on the live site.
    """
    port = _free_port()
    handler = functools.partial(
        http.server.SimpleHTTPRequestHandler, directory=str(directory)
    )
    httpd = socketserver.TCPServer(("127.0.0.1", port), handler)
    thread = threading.Thread(target=httpd.serve_forever, daemon=True)
    thread.start()
    try:
        yield f"http://127.0.0.1:{port}"
    finally:
        httpd.shutdown()
        httpd.server_close()


def _build_site() -> None:
    print("• mkdocs build …", flush=True)
    subprocess.run(
        [sys.executable, "-m", "mkdocs", "build", "--clean", "--quiet"],
        cwd=REPO_ROOT,
        check=True,
    )


def _render(base_url: str, langs: list[str], out_dir: Path) -> list[Path]:
    from playwright.sync_api import sync_playwright  # deferred: optional dep

    out_dir.mkdir(parents=True, exist_ok=True)
    written: list[Path] = []

    with sync_playwright() as p:
        browser = p.chromium.launch()
        for lang in langs:
            rel = LANG_PATHS[lang]
            if not (SITE_DIR / rel).exists():
                print(f"  ! skip {lang}: {rel} not found in build", flush=True)
                continue
            page = browser.new_page()
            page.goto(f"{base_url}/{rel}", wait_until="networkidle")
            page.add_style_tag(content=PRINT_CSS)
            # Hyphen, not dot: a "cv.ko.pdf" name would collide with the i18n
            # plugin's locale-suffix scheme (it'd read it as a translation).
            out = out_dir / ("cv.pdf" if lang == "en" else f"cv-{lang}.pdf")
            page.pdf(
                path=str(out),
                format="A4",
                print_background=True,
                margin={"top": "14mm", "bottom": "14mm", "left": "12mm", "right": "12mm"},
            )
            page.close()
            print(f"  ✓ {lang} -> {out.relative_to(REPO_ROOT)}", flush=True)
            written.append(out)
        browser.close()
    return written


def main() -> int:
    ap = argparse.ArgumentParser(description="Render the CV page to PDF.")
    ap.add_argument(
        "--lang",
        choices=["en", "ko", "both"],
        default="both",
        help="which CV language to render (default: both)",
    )
    ap.add_argument(
        "--out",
        default=str(REPO_ROOT / "docs"),
        help="output directory (default: docs/, so the PDFs are published with the site)",
    )
    ap.add_argument(
        "--no-build",
        action="store_true",
        help="skip `mkdocs build` and use the existing site/ directory",
    )
    args = ap.parse_args()

    langs = ["en", "ko"] if args.lang == "both" else [args.lang]

    if not args.no_build:
        _build_site()
    elif not SITE_DIR.exists():
        print("error: --no-build set but site/ does not exist", file=sys.stderr)
        return 1

    with _serve(SITE_DIR) as base_url:
        written = _render(base_url, langs, Path(args.out).resolve())

    if not written:
        print("error: nothing rendered", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
