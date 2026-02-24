# MacBook Developer Setup Guide (From Zero)

This guide sets up a fresh MacBook for software development, written for engineers coming from a Windows/WSL background.

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

Homebrew is the macOS equivalent of `apt` on Linux/WSL.

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

## 5. Terminal Upgrade

macOS uses **zsh** by default. Install Oh My Zsh for a better shell experience.

```bash
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
```

This adds Git branch display in your prompt, tab completion, and aliases like `gst` (`git status`) and `gco` (`git checkout`).

```bash
# (Optional) Install iTerm2 — a better terminal app than Terminal.app
brew install --cask iterm2
```

---

## 6. Git Configuration

```bash
git config --global user.name "Your Name"
git config --global user.email "your@email.com"
git config --global core.editor "code --wait"
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

## 7. Core Developer Tools

```bash
brew install node python uv gh
```

```bash
# Verify
node --version    # v22.x.x
python3 --version # Python 3.x.x
uv --version      # uv 0.x.x
gh --version      # gh version 2.x.x
```

```bash
# Authenticate GitHub CLI
gh auth login
# Choose: GitHub.com → SSH → Login with a web browser
```

---

## 8. Node Version Management

> Different projects often require different Node versions. Use `nvm` to switch between them.

```bash
brew install nvm

# Add to ~/.zshrc
echo 'export NVM_DIR="$HOME/.nvm"' >> ~/.zshrc
echo '[ -s "/opt/homebrew/opt/nvm/nvm.sh" ] && \. "/opt/homebrew/opt/nvm/nvm.sh"' >> ~/.zshrc
source ~/.zshrc

# Install LTS version
nvm install --lts
nvm use --lts
```

---

## 9. Python Environment

> Never use the system Python (`/usr/bin/python3`). Always use virtual environments per project.

### Option A: venv

```bash
python3 -m venv .venv
source .venv/bin/activate
```

### Option B: uv (faster)

```bash
uv venv
source .venv/bin/activate
```

---

## 10. VS Code

```bash
brew install --cask visual-studio-code
```

Enable the `code` terminal command: open VS Code → `Cmd+Shift+P` → **Shell Command: Install 'code' command in PATH**

```bash
# Verify
code --version

# Open any project from terminal
code ~/Projects/my-project
```

---

## 11. Claude Code Setup

Follow the dedicated guide for Claude Code and MCP servers:

→ **[Mac_Setup_Claude.md](./Mac_Setup_Claude.md)**

Covers: installing `claude`, and adding the Sequential Thinking, GitHub, and Serena MCP servers.

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
- [ ] Oh My Zsh installed
- [ ] Git configured with name, email, and SSH key added to GitHub
- [ ] `node`, `python`, `uv`, `gh` installed
- [ ] GitHub CLI authenticated (`gh auth status`)
- [ ] VS Code installed with `code` command in PATH
- [ ] Claude Code configured (see `Mac_Setup_Claude.md`)
