# MacBook Developer Setup Guide (2026, M4)

This guide sets up a fresh MacBook Pro M4 for software development, written for engineers coming from a Windows/WSL background.

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

Install all GUI applications in one command:

```bash
brew install --cask \
  arc \
  google-chrome \
  firefox \
  cursor \
  claude \
  docker-desktop \
  jetbrains-toolbox \
  warp \
  iterm2
```

| App | Purpose |
|---|---|
| **Arc** | Primary browser |
| **Google Chrome** | Secondary browser / DevTools |
| **Firefox** | Cross-browser testing |
| **Cursor** | AI-powered code editor |
| **Claude** | Claude desktop app |
| **Docker Desktop** | Container runtime |
| **JetBrains Toolbox** | Manage IntelliJ IDEs |
| **Warp** | Modern GPU-accelerated terminal |
| **iTerm2** | Terminal with deep shell integration |

---

## 6. Core Formulae

```bash
brew install uv pixi node
```

| Tool | Purpose |
|---|---|
| **uv** | Fast Python package and project manager |
| **pixi** | Conda-based cross-language environment manager |
| **node** | JavaScript runtime |

```bash
# Verify
uv --version      # uv 0.x.x
pixi --version    # pixi 0.x.x
node --version    # v22.x.x
```

---

## 7. Claude Code CLI

Follow the dedicated guide for Claude Code and MCP servers:

→ **[Mac_Setup_Claude.md](./Mac_Setup_Claude.md)**

Covers: installing the Claude Code native binary, and adding Sequential Thinking, GitHub, and Serena MCP servers.

---

## 8. iTerm2 Finalization

For full iTerm2 configuration (shell integration, status bar, hotkey window, fonts, shortcuts), see the dedicated guide:

→ **[Mac_Setup_iTerm2.md](./Mac_Setup_iTerm2.md)**

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

### uv — Python projects

```bash
# Create a virtual environment
uv venv
source .venv/bin/activate

# Install packages
uv pip install <package>
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

---

## 11. Node

Node is installed as a standalone formula. For projects requiring a specific version, declare it in `package.json` `engines` or a `.nvmrc` and let your editor enforce it.

```bash
node --version    # v22.x.x
npm --version     # 10.x.x
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
- [ ] All casks installed (Arc, Chrome, Firefox, Cursor, Claude, Docker, JetBrains Toolbox, Warp, iTerm2)
- [ ] Core formulae installed (`uv --version`, `pixi --version`, `node --version` all work)
- [ ] Claude Code CLI configured (see `Mac_Setup_Claude.md`)
- [ ] iTerm2 Shell Integration installed, `.hushlogin` created
- [ ] Git configured with name, email, SSH key added to GitHub
