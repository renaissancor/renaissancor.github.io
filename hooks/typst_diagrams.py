"""MkDocs hook: render ```typst fenced blocks to inline SVG at build time.

Each fence body is wrapped in a small preamble (auto-sized page, transparent
background, neutral font), compiled with the local `typst` binary, cached by
content hash under .cache/typst-diagrams/, and inlined into the page as SVG.
Black strokes/fills are rewritten to `currentColor` so the diagram follows the
theme's text colour (light and dark palettes both work).

Failure never aborts the build: a compile error is rendered in place as a
visible error box with typst's stderr, so a broken diagram is impossible to
miss but the rest of the site still ships.
"""
from __future__ import annotations

import hashlib
import html
import re
import shutil
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CACHE = ROOT / ".cache" / "typst-diagrams"

PREAMBLE = """\
#set page(width: auto, height: auto, margin: 6pt, fill: none)
#set text(font: ("Helvetica Neue", "Helvetica", "Apple SD Gothic Neo", "Noto Sans CJK KR"), size: 10pt)
#set par(justify: false)
"""

FENCE = re.compile(r"^```typst[^\n]*\n(.*?)^```[ \t]*$", re.MULTILINE | re.DOTALL)


def _render(body: str) -> str:
    src = PREAMBLE + body
    key = hashlib.sha256(src.encode()).hexdigest()[:16]
    CACHE.mkdir(parents=True, exist_ok=True)
    svg_path = CACHE / f"{key}.svg"
    if not svg_path.exists():
        typ_path = CACHE / f"{key}.typ"
        typ_path.write_text(src, encoding="utf-8")
        typst = shutil.which("typst")
        if typst is None:
            return _error("`typst` binary not found on PATH", body)
        proc = subprocess.run(
            [typst, "compile", "--format", "svg", "--root", str(ROOT), str(typ_path), str(svg_path)],
            capture_output=True, text=True,
        )
        if proc.returncode != 0:
            svg_path.unlink(missing_ok=True)
            return _error(proc.stderr.strip() or "typst compile failed", body)
    svg = svg_path.read_text(encoding="utf-8")
    svg = svg.replace('"#000000"', '"currentColor"')
    return f'<div class="typst-diagram">{svg}</div>'


def _error(msg: str, body: str) -> str:
    return (
        '<div class="typst-diagram typst-error"><strong>Typst diagram failed</strong>'
        f"<pre>{html.escape(msg)}</pre><pre>{html.escape(body)}</pre></div>"
    )


def on_page_markdown(markdown: str, **kwargs) -> str:
    if "```typst" not in markdown:
        return markdown
    return FENCE.sub(lambda m: "\n" + _render(m.group(1)) + "\n", markdown)
