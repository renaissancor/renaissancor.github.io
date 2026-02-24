# Linux Developer Setup Guide (From Zero)

This guide sets up a fresh Ubuntu/Debian Linux machine for software development, including Claude Code with GitHub integration, Sequential Thinking, and the Serena agent.

> Commands use `apt`. For **Fedora/RHEL** use `dnf`, for **Arch** use `pacman`.

---

## 1. System Update & Build Tools

```bash
sudo apt update && sudo apt upgrade -y && sudo apt autoremove -y

# Install compilers, make, and other build dependencies
sudo apt install -y build-essential curl git wget unzip ca-certificates gnupg
```

---

## 2. Shell: zsh + Oh My Zsh

> Ubuntu defaults to **bash**. zsh gives you Git branch display, tab completion, and useful aliases.

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

## 3. Git Configuration

```bash
git config --global user.name "Your Name"
git config --global user.email "your@email.com"
git config --global core.editor "code --wait"
git config --global init.defaultBranch main
```

### SSH Key for GitHub

```bash
# Install clipboard tool
sudo apt install -y xclip

# Generate key (use your GitHub email)
ssh-keygen -t ed25519 -C "your@email.com"
# Press Enter three times to accept defaults

# Copy public key to clipboard
xclip -selection clipboard < ~/.ssh/id_ed25519.pub
```

Go to [GitHub → Settings → SSH Keys → New SSH Key](https://github.com/settings/keys), paste, and save.

```bash
# Test the connection
ssh -T git@github.com
# Expected: Hi username! You've successfully authenticated...
```

---

## 4. Node.js (via nvm)

> **Do NOT** use `sudo apt install nodejs` — it installs an outdated version.

```bash
# Install nvm
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.1/install.sh | bash
source ~/.zshrc

# Install latest LTS
nvm install --lts
nvm use --lts

# Verify (should be v22+)
node -v
```

---

## 5. Python Environment

> Never use the system Python. Always use virtual environments per project.

```bash
# Install Python and pip
sudo apt install -y python3 python3-pip python3-venv
```

Choose **one** of the following per project:

### Option A: venv

```bash
python3 -m venv .venv
source .venv/bin/activate
```

### Option B: Conda

```bash
# Install Miniconda (if not already installed)
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh
source ~/.zshrc

conda create -n myenv python=3.10 -y
conda activate myenv
```

---

## 6. uv (Python/Tool Manager)

```bash
curl -LsSf https://astral.sh/uv/install.sh | sh
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc

# Verify
uv --version
```

---

## 7. GitHub CLI

```bash
# Add the official GitHub CLI apt repository
sudo mkdir -p -m 755 /etc/apt/keyrings
wget -qO- https://cli.github.com/packages/githubcli-archive-keyring.gpg \
  | sudo tee /etc/apt/keyrings/githubcli-archive-keyring.gpg > /dev/null
sudo chmod go+r /etc/apt/keyrings/githubcli-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/githubcli-archive-keyring.gpg] \
  https://cli.github.com/packages stable main" \
  | sudo tee /etc/apt/sources.list.d/github-cli.list > /dev/null

sudo apt update && sudo apt install -y gh

# Authenticate
gh auth login
# Choose: GitHub.com → SSH → Login with a web browser
```

---

## 8. VS Code

```bash
# Install via Snap (simplest method)
sudo snap install code --classic

# Verify
code --version
```

The `code` command is available in your terminal immediately after install.

```bash
# Open any project from terminal
code ~/Projects/my-project
```

### Recommended Extensions

```bash
code --install-extension ms-python.python
code --install-extension dbaeumer.vscode-eslint
code --install-extension esbenp.prettier-vscode
code --install-extension eamodio.gitlens
code --install-extension github.copilot
```

---

## 9. Verify All Installations

```bash
node -v && python3 --version && uv --version && gh --version && code --version
```

---

## 10. Claude Code

```bash
npm install -g @anthropic-ai/claude-code
claude
```

Select your login method when prompted:

1. **Claude account** — Pro, Max, Team, or Enterprise
2. **Anthropic Console** — API usage billing
3. **3rd-party platform** — Amazon Bedrock, Microsoft Foundry, or Vertex AI

---

## 11. MCP Server: Sequential Thinking

Enables Claude to break down complex problems into step-by-step reasoning.

```bash
claude mcp add sequential-thinking -- npx -y @modelcontextprotocol/server-sequential-thinking
```

---

## 12. MCP Server: GitHub

Connects Claude to your repositories for PR management, issue tracking, and code search.

### Step A: Generate a GitHub Token

1. Go to [GitHub Personal Access Tokens (Classic)](https://github.com/settings/tokens).
2. **Required Scopes:** `repo`, `read:org`, `user:email`, `workflow` (optional)

### Step B: Configure Environment

```bash
# Add token to shell profile
echo 'export GITHUB_PERSONAL_ACCESS_TOKEN="ghp_your_token_here"' >> ~/.zshrc
source ~/.zshrc
```

### Step C: Add to Claude

```bash
claude mcp add github -- npx -y @modelcontextprotocol/server-github
```

---

## 13. MCP Server: Serena

Serena is a professional coding agent. The web dashboard must be disabled in terminal environments to prevent timeouts.

### Step A: Install Permanently

```bash
uv tool install git+https://github.com/oraios/serena
uv tool update-shell
source ~/.zshrc
```

### Step B: Headless Configuration

```bash
mkdir -p ~/.serena

cat << EOF > ~/.serena/serena_config.yml
web_dashboard: false
web_dashboard_open_on_launch: false
projects:
  - "/home/$(whoami)/Project"
EOF
```

### Step C: Add to Claude

```bash
claude mcp add serena -- /home/$(whoami)/.local/bin/serena start-mcp-server
```

---

## 14. Optional Tools

### FFmpeg

```bash
sudo apt install -y ffmpeg
ffmpeg -version
```

### Actionbook (Browser Access from Terminal)

```bash
npm install -g @actionbookdev/cli
sudo apt install -y chromium-browser
```

See: [github.com/actionbook/actionbook](https://github.com/actionbook/actionbook)

---

## 15. Expected `~/.claude.json`

Your final configuration should look like:

```json
{
  "mcpServers": {
    "sequential-thinking": {
      "command": "npx",
      "args": ["-y", "@modelcontextprotocol/server-sequential-thinking"]
    },
    "github": {
      "command": "npx",
      "args": ["-y", "@modelcontextprotocol/server-github"],
      "env": {
        "GITHUB_PERSONAL_ACCESS_TOKEN": "ghp_your_token_here"
      }
    },
    "serena": {
      "command": "/home/your_username/.local/bin/serena",
      "args": ["start-mcp-server"]
    }
  }
}
```

---

## 16. Verification

```bash
claude
❯ /mcp list
```

All status indicators should be **green**.

### Troubleshooting

| Issue | Solution |
|---|---|
| **Serena timeout** | Ensure `web_dashboard: false` in `~/.serena/serena_config.yml` |
| **Path errors (Serena)** | Use simple strings in `projects` (e.g., `"/home/name/Project"`), not maps with `name:` or `path:` keys |
| **Node not found** | Run `nvm use --lts` or restart your shell |
| **zsh not default** | Run `chsh -s $(which zsh)` then log out and back in |
| **`code` not found** | Run `sudo snap install code --classic` or restart your terminal |
| **`xclip` not found** | Run `sudo apt install -y xclip` |

---

## Linux vs macOS vs WSL — Key Differences

| | Linux Native | macOS | WSL (Windows) |
|---|---|---|---|
| Package manager | `apt` / `dnf` / `pacman` | `brew` | `apt` |
| Home directory | `/home/username` | `/Users/username` | `/home/username` |
| Copy to clipboard | `xclip -selection clipboard` | `pbcopy` | `clip.exe` |
| Open folder in GUI | `xdg-open .` | `open .` | `explorer.exe .` |
| Default shell | bash (change to zsh) | zsh | bash or zsh |
| Shell profile | `~/.zshrc` | `~/.zshrc` + `~/.zprofile` | `~/.zshrc` |
| VS Code install | `snap install code` | `brew install --cask` | Windows installer |
| System Python | Avoid | Avoid | Avoid |

---

## Checklist

- [ ] System updated (`apt update && apt upgrade`)
- [ ] `build-essential` and `git` installed
- [ ] zsh installed and set as default shell (`echo $SHELL` → `/usr/bin/zsh`)
- [ ] Oh My Zsh installed
- [ ] Git configured with name, email, and SSH key added to GitHub
- [ ] `node` installed via nvm (v22+)
- [ ] `python3`, `uv` installed
- [ ] GitHub CLI installed and authenticated (`gh auth status`)
- [ ] VS Code installed (`code --version` works)
- [ ] Claude Code installed and logged in (`claude`)
- [ ] All three MCP servers green (`/mcp list`)
