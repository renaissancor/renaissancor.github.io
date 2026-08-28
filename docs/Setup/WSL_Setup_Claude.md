# WSL Dev Setup Guide (Windows + Ubuntu)

This guide sets up **WSL2 (Ubuntu)** on my personal Windows desktop as a Linux dev
environment with **Claude Code** and the AI harness. It doubles as the generic Linux setup
guide — everything from Section 2 onward applies to a native Ubuntu machine too; skip the
WSL-specific parts (Sections 1 and 9).

> The AI-harness layer (plugins, Codex offload, rtk, Serena, skills) is documented once in
> **[Mac_Setup_Claude.md](./Mac_Setup_Claude.md)** — the concepts and commands are
> platform-independent. This guide covers the Linux-side installs and the WSL-specific
> pitfalls.

---

## 1. Install WSL2 (Windows Side)

> Run these commands in **PowerShell (Admin)** — right-click Start → Terminal (Admin). Everything after this step runs inside WSL.

```powershell
# Install WSL2 + Ubuntu (if not already present)
wsl --install -d Ubuntu

# Keep WSL and the kernel up to date
wsl --update
```

Verify WSL2 is active:

```powershell
wsl --list --verbose
# VERSION column must be 2 for Ubuntu
```

If it shows version 1, convert: `wsl --set-version Ubuntu 2`.

Launch Ubuntu once to create your Linux user, then close PowerShell. **All remaining steps run inside the WSL terminal.**

> **Performance rule:** Always keep projects under `~/` inside WSL. The `/mnt/c/` mount is Windows NTFS bridged through WSL — it is **10-100x slower** for git, node_modules, and file-watching. Never clone repos to `/mnt/c/Users/...`.

---

## 2. System Update & Build Tools

```bash
sudo apt update && sudo apt upgrade -y && sudo apt autoremove -y

# Install compilers and core tools needed by nvm, uv, and other installers
sudo apt install -y build-essential curl git wget unzip ca-certificates gnupg
```

---

## 3. Shell: zsh + Oh My Zsh

> Ubuntu defaults to **bash**. zsh gives you Git branch display, tab completion, and useful aliases. This step must come before anything that writes to `~/.zshrc`.

```bash
# Install zsh
sudo apt install -y zsh

# Set zsh as your default shell (takes effect on next login)
chsh -s $(which zsh)

# Install Oh My Zsh
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
```

Close and reopen your terminal. You should now be in zsh.

```bash
# Verify
echo $SHELL
# Expected: /usr/bin/zsh
```

---

## 4. Git + GitHub

```bash
git config --global user.name "Your Name"
git config --global user.email "your@email.com"
git config --global init.defaultBranch main

# Generate an SSH key (use your GitHub email), then add the .pub to GitHub → Settings → SSH Keys
ssh-keygen -t ed25519 -C "your@email.com"
cat ~/.ssh/id_ed25519.pub

# Test
ssh -T git@github.com
```

### GitHub CLI

`gh` handles PRs, issues, and API calls from the terminal — and Claude Code drives it
natively, which is why no GitHub MCP server is needed anymore.

```bash
sudo mkdir -p -m 755 /etc/apt/keyrings
wget -qO- https://cli.github.com/packages/githubcli-archive-keyring.gpg \
  | sudo tee /etc/apt/keyrings/githubcli-archive-keyring.gpg > /dev/null
sudo chmod go+r /etc/apt/keyrings/githubcli-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/githubcli-archive-keyring.gpg] \
  https://cli.github.com/packages stable main" \
  | sudo tee /etc/apt/sources.list.d/github-cli.list > /dev/null

sudo apt update && sudo apt install -y gh
gh auth login   # GitHub.com → SSH → Login with a web browser
```

---

## 5. Node.js (via nvm)

> **Do NOT** use `sudo apt install nodejs` — it installs an outdated version.

```bash
# Install nvm (check https://github.com/nvm-sh/nvm for the current version tag)
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.1/install.sh | bash
source ~/.zshrc

# Install latest LTS
nvm install --lts
nvm use --lts

# Verify (v24+ LTS as of 2026)
node -v
```

---

## 6. Python: uv

> Never use the system Python. `uv` is the default for every Python project; skip conda
> unless a specific project demands it.

```bash
curl -LsSf https://astral.sh/uv/install.sh | sh
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc

# Verify
uv --version

# Per-project workflow
uv venv && source .venv/bin/activate   # classic venv
uv sync                                # or: locked project from pyproject.toml + uv.lock
uv run python script.py                # run without activating
```

---

## 7. Claude Code

```bash
# Install Claude Code CLI (native binary, self-updating, lands in ~/.local/bin)
curl -fsSL https://claude.ai/install.sh | bash

# Initialize (follow the login prompts)
claude
```

Log in with a **Claude account** (Pro/Max/Team/Enterprise) unless you specifically want
API-metered billing.

### The harness layer

Everything beyond the bare install — **plugins** (oh-my-claudecode, codex), the **Codex
CLI** as a token-offload second engine, **rtk**, **skills**, the **CLAUDE.md hierarchy**,
and `settings.json` permissions — is documented in
**[Mac_Setup_Claude.md](./Mac_Setup_Claude.md)** and works identically on Linux/WSL.
Linux-specific notes:

```bash
# Codex CLI on Linux (no brew cask here) — install via npm, log in with ChatGPT account
npm install -g @openai/codex
codex login
```

### Serena MCP (per-project)

Register Serena in a committed `.mcp.json` at the repo root rather than globally — the
machine stays reproducible from the repo alone:

```json
{
  "mcpServers": {
    "serena": {
      "command": "uvx",
      "args": [
        "--from", "git+https://github.com/oraios/serena",
        "serena", "start-mcp-server", "--context", "ide-assistant"
      ]
    }
  }
}
```

Verify from any session:

```bash
claude mcp list
```

> **Dropped since the February version of this guide:** Sequential Thinking MCP (built-in
> extended thinking covers it), Sentry MCP (never used), GitHub MCP (`gh` does it better).

---

## 8. Project Memory (CLAUDE.md)

`CLAUDE.md` is read automatically at session start — project conventions, build/test
commands, and gotchas live there so Claude never needs to be told twice.

```bash
# Inside your project directory, inside Claude Code:
❯ /init
```

Keep it concise; delete anything Claude can infer from the code. Details and the global
`~/.claude/CLAUDE.md` hierarchy: see [Mac_Setup_Claude.md](./Mac_Setup_Claude.md).

---

## 9. Windows Interop

### Clipboard — `pbcopy` / `pbpaste` shims

Many guides and workflows reference `pbcopy` (macOS). On WSL, use Windows clipboard interop:

```bash
# Add to ~/.zshrc
alias pbcopy='clip.exe'
alias pbpaste='powershell.exe -NoProfile -Command Get-Clipboard | tr -d "\r"'
```

`clip.exe` reads stdin and puts it on the Windows clipboard — `echo "foo" | pbcopy` works exactly like on macOS.

### Opening URLs and folders in Windows

```bash
# Install wslu (Windows Subsystem for Linux Utilities) if not already present
sudo apt install -y wslu

wslview https://example.com      # opens default Windows browser
explorer.exe .                   # opens current WSL dir in Windows Explorer
```

### Editors / IDEs

- **VS Code**: Install on Windows, then add the **WSL** extension. Run `code .` inside WSL — the VS Code server runs in WSL, the UI runs on Windows. This is the recommended setup.
- **JetBrains (PyCharm/IntelliJ)**: Use "Remote Development → WSL" from the Welcome screen. Don't open WSL projects via `\\wsl$\...` in the Windows IDE — that breaks file watchers.

### SSH keys

Keep SSH keys inside WSL, not on the Windows filesystem:

```bash
# If you have existing keys on Windows, copy them in:
cp /mnt/c/Users/YOUR_USERNAME/.ssh/id_ed25519{,.pub} ~/.ssh/
chmod 700 ~/.ssh
chmod 600 ~/.ssh/id_ed25519
chmod 644 ~/.ssh/id_ed25519.pub
```

Permissions on `/mnt/c` are faked by WSL and will break SSH's strict-mode check — always store keys under `~/.ssh/` inside WSL.

---

## 10. Optional Tools

```bash
sudo apt install -y ffmpeg jq ripgrep tmux
```

---

## 11. Verification

```bash
node -v && uv --version && gh --version && claude --version && codex --version
claude
❯ /mcp        # MCP servers green
❯ /plugin     # plugins enabled
```

---

## Troubleshooting

| Issue | Solution |
|---|---|
| **WSL version is 1** | From PowerShell: `wsl --set-version Ubuntu 2` |
| **Slow git/npm operations** | Move your project to `~/` inside WSL — `/mnt/c` is 10-100x slower |
| **Serena timeout** | Use `--context ide-assistant` in the `.mcp.json` args |
| **`claude` not found** | The installer puts it in `~/.local/bin` — ensure that's on PATH |
| **Node not found** | Run `nvm use --lts` or restart your shell |
| **zsh not default** | Run `chsh -s $(which zsh)` then close and reopen WSL |
| **`~/.zshrc` not found** | Make sure Oh My Zsh is installed before running the Node/uv installers |
| **WSL paths** | Use `/home/username/...` not `/Users/username/...` (that's macOS) |
| **Clipboard not working** | Ensure `alias pbcopy='clip.exe'` is in `~/.zshrc` and restart your shell |
| **VS Code can't see WSL** | Install the **WSL** extension in VS Code on the Windows side |

---

## Checklist

- [ ] WSL2 + Ubuntu installed, projects under `~/` (never `/mnt/c`)
- [ ] `build-essential` and `git` installed
- [ ] zsh + Oh My Zsh installed and default
- [ ] Git configured, SSH key added to GitHub, `gh` authenticated
- [ ] `node` via nvm (v24+ LTS), `uv` installed
- [ ] Claude Code installed and logged in
- [ ] Harness layer configured per [Mac_Setup_Claude.md](./Mac_Setup_Claude.md) (plugins, Codex, Serena)
- [ ] Clipboard shims and wslu installed

---

_Last updated: August 2026 — harness layer consolidated into Mac_Setup_Claude.md; stale
MCP recipe (Sequential Thinking / Sentry / GitHub) removed. This guide also serves as the
native-Ubuntu setup reference (the former Linux_Dev_Setup.md)._
