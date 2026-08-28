# MacBook Developer Setup Guide (2026, M4 Pro)

This guide rebuilds my MacBook Pro (M4 Pro, macOS 26) development environment from scratch.
Originally written for engineers coming from a Windows/WSL background; as of August 2026 it
reflects the machine I actually work on daily — an ML/data-infrastructure setup centered on
terminals, Python tooling, and an AI coding harness.

---

## 1. Security (Do This First)

> Your MacBook shipped with a weak password. Fix this before anything else.

1. **Apple Menu → System Settings → Touch ID & Password** → change your password
2. **System Settings → Privacy & Security → FileVault** → Turn On (full-disk encryption)

---

## 2. System Settings

### Trackpad
**System Settings → Trackpad:**
- Enable **Tap to click**
- Enable **Three finger drag** (Accessibility → Pointer Control → Trackpad Options)

### Keyboard
**System Settings → Keyboard:**
- Set **Key repeat rate** to Fast
- Set **Delay until repeat** to Short

### Finder: Show Hidden Files
```bash
defaults write com.apple.finder AppleShowAllFiles YES && killall Finder
```

---

## 3. Xcode Command Line Tools

> This installs Git and the compilers that every other developer tool depends on. Run this first.

```bash
xcode-select --install
```

A popup will appear — click **Install** and wait (~5 minutes).

```bash
# Verify
git --version
```

---

## 4. Homebrew

Homebrew is the macOS package manager for both GUI apps (casks) and CLI tools (formulae).

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

> After installation, the installer prints two commands to add Homebrew to your PATH. **Run both of them**, then:

```bash
source ~/.zprofile

# Verify
brew --version
```

---

## 5. Apps via Homebrew Cask

This is the full set I run today, grouped by role. Install what you need in one command:

```bash
# Browsers
brew install --cask arc google-chrome firefox

# Terminals — iTerm2 is my daily driver; Ghostty and Warp as alternates
brew install --cask iterm2 ghostty warp

# Editors & IDEs
brew install --cask visual-studio-code cursor zed jetbrains-toolbox

# AI apps — desktop clients + CLI-bundled casks
brew install --cask claude chatgpt codex antigravity

# Dev utilities
brew install --cask docker-desktop github db-browser-for-sqlite

# Quality of life
brew install --cask rectangle maccy keyclu aldente

# Documents & notes
brew install --cask obsidian typora skim basictex libreoffice

# Communication
brew install --cask zoom
```

| App | Purpose |
|---|---|
| **Arc** | Primary browser |
| **iTerm2** | Primary terminal (see [iterm2_setup.md](./iterm2_setup.md)) |
| **Ghostty / Warp** | Alternate terminals — Ghostty for speed, Warp for AI features |
| **VS Code / Cursor / Zed** | Editors; JetBrains Toolbox manages DataGrip/PyCharm |
| **Claude** | Claude desktop app (the CLI is separate — see below) |
| **ChatGPT / Codex** | ChatGPT desktop + Codex CLI (the `codex` cask bundles the CLI) |
| **Antigravity** | Google's agentic IDE |
| **Docker Desktop** | Container runtime |
| **GitHub Desktop** | Occasional GUI git review |
| **Rectangle** | Window snapping |
| **Maccy** | Clipboard history — essential when tools fight over the clipboard |
| **KeyClu** | Shortcut cheat-sheet overlay |
| **AlDente** | Battery charge limiter |
| **Obsidian / Typora** | Markdown notes / editing |
| **Skim + BasicTeX** | PDF reading + LaTeX |

---

## 6. Core Formulae

```bash
# Language & environment managers
brew install uv pixi node

# Everyday CLI tools
brew install gh jq ripgrep tmux ffmpeg

# Data engines (local analysis / mirrors of production stores)
brew install duckdb postgresql@16 mysql
```

| Tool | Purpose |
|---|---|
| **uv** | Fast Python package and project manager — the default for every Python project |
| **pixi** | Conda-based cross-language environment manager (GPU/ML projects) |
| **node** | JavaScript runtime (v26.x as of August 2026) |
| **gh** | GitHub CLI — PRs, issues, API from the terminal |
| **jq / ripgrep / tmux** | JSON wrangling, fast search, persistent sessions |
| **ffmpeg** | Media pipelines (frame/audio extraction) |
| **duckdb** | Analytical SQL over Parquet — the local data-mart engine |
| **postgresql@16 / mysql** | Local mirrors for verification labs |

```bash
# Verify
uv --version && pixi --version && node --version
```

---

## 7. Claude Code + AI Harness

The AI tooling (Claude Code CLI, plugins, MCP servers, Codex CLI, rtk) has its own guide:

→ **[Harness_setup.md](./Harness_setup.md)**

Covers: the Claude Code native binary, the oh-my-claudecode plugin, the Codex CLI offload
harness, the rtk token proxy, Serena MCP, skills, and scheduled automation.

---

## 8. iTerm2 Finalization

For full iTerm2 configuration (shell integration, status bar, hotkey window, fonts, shortcuts), see:

→ **[iterm2_setup.md](./iterm2_setup.md)**

The two steps to do immediately after installing:

```bash
# 1. Install Shell Integration (or use iTerm2 menu → Install Shell Integration)
curl -L https://iterm2.com/shell_integration/install_shell_integration.sh | bash
source ~/.zshrc

# 2. Suppress the login banner
touch ~/.hushlogin
```

---

## 9. Git Configuration

```bash
git config --global user.name "Your Name"
git config --global user.email "your@email.com"
git config --global init.defaultBranch main
```

### SSH Key for GitHub

```bash
# Generate key (use your GitHub email)
ssh-keygen -t ed25519 -C "your@email.com"
# Press Enter three times to accept defaults

# Copy public key to clipboard
cat ~/.ssh/id_ed25519.pub | pbcopy
```

Go to [GitHub → Settings → SSH Keys → New SSH Key](https://github.com/settings/keys), paste, and save.

```bash
# Test the connection
ssh -T git@github.com
# Expected: Hi username! You've successfully authenticated...
```

---

## 10. Python Environments

> Never use the system Python (`/usr/bin/python3`). Manage environments per project.

### uv — Python projects (the default)

```bash
# Create a virtual environment
uv venv
source .venv/bin/activate

# Install packages
uv pip install <package>

# Or, for a locked project workflow:
uv sync            # install from pyproject.toml + uv.lock
uv run python x.py # run inside the environment without activating
```

### pixi — multi-language / conda-based projects

```bash
# Initialize a project
pixi init my-project
cd my-project

# Add packages from conda-forge or PyPI
pixi add python numpy pandas

# Run inside the managed environment
pixi run python script.py
```

### uv tool — Global Python CLI Tools

Use `uv tool install` for CLI tools you want available globally (modern replacement for `pipx`). The tool runs in its own isolated environment — no virtual env activation needed.

```bash
# Example: MkDocs for static site generation (this site)
uv tool install "mkdocs>=1.6,<2.0" --with mkdocs-material

# Verify
mkdocs --version
```

```bash
# Common MkDocs commands (run from your docs project root)
mkdocs serve        # local preview at http://127.0.0.1:8000
mkdocs build        # build static site to /site
mkdocs gh-deploy    # deploy directly to GitHub Pages
```

> **Why pin `<2.0`?** MkDocs 2.0 broke compatibility with `mkdocs-material` when it landed
> in early 2026. I still run MkDocs 1.6.x — re-check Material's compatibility notes before
> unpinning.

---

## 11. Node

Node is installed as a standalone formula. For projects requiring a specific version, declare it in `package.json` `engines` or a `.nvmrc` and let your editor enforce it.

```bash
node --version    # v26.x
npm --version
```

---

## macOS vs WSL — Key Differences

| | WSL (Windows) | macOS |
|---|---|---|
| Package manager | `apt` | `brew` |
| Home directory | `/home/username` | `/Users/username` |
| Copy to clipboard | `clip.exe` | `pbcopy` |
| Open folder in GUI | `explorer.exe .` | `open .` |
| Shell profile | `~/.zshrc` | `~/.zshrc` + `~/.zprofile` |
| System Python | Avoid | Avoid |

---

## Checklist

- [ ] Password changed, FileVault enabled
- [ ] Xcode Command Line Tools installed (`git --version` works)
- [ ] Homebrew installed and on PATH (`brew --version` works)
- [ ] Casks installed (browsers, terminals, editors, AI apps, utilities)
- [ ] Core formulae installed (`uv`, `pixi`, `node`, `gh`, `duckdb` all on PATH)
- [ ] MkDocs installed via uv tool (`mkdocs --version` works)
- [ ] Claude Code + AI harness configured (see `Harness_setup.md`)
- [ ] iTerm2 Shell Integration installed, `.hushlogin` created
- [ ] Git configured with name, email, SSH key added to GitHub

---

_Last updated: August 2026 — rewritten to match the machine as actually configured._
