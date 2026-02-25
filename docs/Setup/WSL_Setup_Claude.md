# WSL Dev Setup Guide

This guide configures a **Claude Code** terminal environment on Windows (WSL/Ubuntu) with GitHub integration, Sequential Thinking, and the Serena agent.

---

## 1. System Update & Build Tools

```bash
sudo apt update && sudo apt upgrade -y && sudo apt autoremove -y

# Install compilers and core tools needed by nvm, uv, and other installers
sudo apt install -y build-essential curl git wget unzip ca-certificates gnupg
```

---

## 2. Shell: zsh + Oh My Zsh

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

## 3. Node.js (via nvm)

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

## 4. Python Environment

> Never use the system Python. Always use virtual environments per project.

### Option A: uv (recommended)

```bash
curl -LsSf https://astral.sh/uv/install.sh | sh
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc

# Verify
uv --version

# Create a virtual environment in your project
uv venv
source .venv/bin/activate
```

### Option B: Miniconda

```bash
# Download and install Miniconda
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh
# Follow the prompts; answer 'yes' to init when asked

# Initialize for zsh
~/miniconda3/bin/conda init zsh
source ~/.zshrc

# Create an environment
conda create -n myenv python=3.11 -y
conda activate myenv
```

---

## 5. Verify All Installations

```bash
node -v && uv --version
```

If using Conda:

```bash
conda --version
```

---

## 6. Claude Code

```bash
npm install -g @anthropic-ai/claude-code
claude
```

Select your login method when prompted:

1. **Claude account** — Pro, Max, Team, or Enterprise
2. **Anthropic Console** — API usage billing
3. **3rd-party platform** — Amazon Bedrock, Microsoft Foundry, or Vertex AI

---

## 7. MCP Server: Sequential Thinking

Enables Claude to break down complex problems into step-by-step reasoning.

```bash
claude mcp add sequential-thinking -- npx -y @modelcontextprotocol/server-sequential-thinking
```

---

## 8. MCP Server: GitHub

Connects Claude to your repositories for PR management, issue tracking, and code search.

### Step A: Generate a GitHub Token

1. Go to [GitHub Personal Access Tokens (Classic)](https://github.com/settings/tokens).
2. **Required Scopes:** `repo`, `read:org`, `user:email`, `workflow` (optional)

### Step B: Configure Environment

```bash
echo 'export GITHUB_PERSONAL_ACCESS_TOKEN="ghp_your_token_here"' >> ~/.zshrc
source ~/.zshrc
```

### Step C: Add to Claude

```bash
claude mcp add github -- npx -y @modelcontextprotocol/server-github
```

---

## 9. MCP Server: Serena

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

## 10. Optional Tools

### FFmpeg

```bash
sudo apt install -y ffmpeg
ffmpeg -version
```

---

## 11. Expected `~/.claude.json`

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

## 12. Verification

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
| **zsh not default** | Run `chsh -s $(which zsh)` then close and reopen WSL |
| **`~/.zshrc` not found** | Make sure Oh My Zsh is installed before running the Node/uv installers |
| **WSL paths** | Use `/home/username/...` not `/Users/username/...` (that's macOS) |
