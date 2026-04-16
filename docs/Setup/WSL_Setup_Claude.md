# WSL Dev Setup Guide

This guide configures a **Claude Code** terminal environment on Windows (WSL/Ubuntu) with GitHub integration, Sequential Thinking, and the Serena agent.

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

## 6. Verify All Installations

```bash
node -v && uv --version
```

If using Conda:

```bash
conda --version
```

---

## 7. Claude Code

```bash
# Install Claude Code CLI (native binary, auto-updates)
curl -fsSL https://claude.ai/install.sh | bash

# Initialize (follow the login prompts)
claude
```

Select your login method when prompted:

1. **Claude account** — Pro, Max, Team, or Enterprise
2. **Anthropic Console** — API usage billing
3. **3rd-party platform** — Amazon Bedrock, Microsoft Foundry, or Vertex AI

---

## 8. MCP Server: Sequential Thinking (Optional)

> **Note:** Claude's built-in extended thinking (`/think` during chat) covers most reasoning use cases natively. Add this server only if you want explicit step-by-step tool calls in your workflow.

```bash
claude mcp add sequential-thinking -- npx -y @modelcontextprotocol/server-sequential-thinking
```

---

## 9. MCP Server: GitHub

Connects Claude to your repositories for PR management, issue tracking, and code search. Uses GitHub's official hosted endpoint with OAuth — no personal access token required.

### Step A: Add to Claude

```bash
claude mcp add --transport http github https://api.githubcopilot.com/mcp/
```

### Step B: Authenticate

```bash
# Inside Claude Code, open the MCP menu and follow the browser login flow
claude
❯ /mcp
```

Select **github** → **Authenticate** and complete the GitHub OAuth flow in your browser.

---

## 10. MCP Server: Sentry

Connects Claude to your error monitoring — search issues, inspect stack traces, and debug production errors without leaving the terminal.

### Step A: Add to Claude

```bash
claude mcp add --transport http sentry https://mcp.sentry.dev/mcp
```

### Step B: Authenticate

```bash
claude
❯ /mcp
```

Select **sentry** → **Authenticate** and complete the Sentry OAuth flow.

### Usage examples

```
> What are the top 5 errors in production this week?
> Show me the full stack trace for issue PROJ-1234
> Which errors spiked after the last deploy?
```

---

## 11. MCP Server: Serena

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

## 12. Project Memory (CLAUDE.md)

`CLAUDE.md` is a file Claude reads automatically at the start of every session in your project. Use it to encode project-specific conventions, commands, and gotchas so Claude never needs to be told twice.

### Initialize

```bash
# Inside your project directory, inside Claude Code:
❯ /init
```

This generates a starter `CLAUDE.md` based on your codebase. Edit it to reflect your actual workflow:

```markdown
# Build & Test
- Build: `npm run build`
- Test single: `npm test -- --testNamePattern="pattern"`
- Lint (auto-fix): `npm run lint:fix`

# Conventions
- Use 2-space indentation
- Prefer `const` over `let`; avoid `var`
- Commit format: `type(scope): description` (e.g. `feat(auth): add OAuth`)

# Gotchas
- DB migrations must be idempotent
- `utils/legacy.ts` is deprecated — use `utils/new.ts`
- Requires `REDIS_URL` and `API_KEY` env vars
```

> Commit `CLAUDE.md` to git — it applies to everyone on the team. Keep it concise; delete anything Claude can already infer from the code.

---

## 13. Permissions & Settings

Claude Code respects a `settings.json` file that controls which commands are allowed or denied, preventing accidental destructive operations.

```bash
mkdir -p ~/.claude
```

Create or edit `~/.claude/settings.json`:

```json
{
  "permissions": {
    "allow": [
      "Bash(npm run *)",
      "Bash(git *)",
      "Bash(docker *)"
    ],
    "deny": [
      "Bash(rm -rf *)",
      "Bash(sudo *)",
      "Read(.env*)"
    ]
  }
}
```

> For team-wide defaults, create `.claude/settings.json` inside your project repo and commit it. User-level `~/.claude/settings.json` takes precedence.

---

## 14. Windows Interop

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

## 15. Optional Tools

### FFmpeg

```bash
sudo apt install -y ffmpeg
ffmpeg -version
```

---

## 16. Expected `~/.claude.json`

Your final configuration should look like:

```json
{
  "mcpServers": {
    "sequential-thinking": {
      "command": "npx",
      "args": ["-y", "@modelcontextprotocol/server-sequential-thinking"]
    },
    "github": {
      "type": "http",
      "url": "https://api.githubcopilot.com/mcp/"
    },
    "sentry": {
      "type": "http",
      "url": "https://mcp.sentry.dev/mcp"
    },
    "serena": {
      "command": "/home/your_username/.local/bin/serena",
      "args": ["start-mcp-server"]
    }
  }
}
```

---

## 17. Verification

```bash
claude
❯ /mcp
```

All status indicators should be **green**.

---

## Troubleshooting

| Issue | Solution |
|---|---|
| **WSL version is 1** | From PowerShell: `wsl --set-version Ubuntu 2` |
| **Slow git/npm operations** | Move your project to `~/` inside WSL — `/mnt/c` is 10-100x slower |
| **Serena timeout** | Ensure `web_dashboard: false` in `~/.serena/serena_config.yml` |
| **Path errors (Serena)** | Use simple strings in `projects` (e.g., `"/home/name/Project"`), not maps with `name:` or `path:` keys |
| **Node not found** | Run `nvm use --lts` or restart your shell |
| **zsh not default** | Run `chsh -s $(which zsh)` then close and reopen WSL |
| **`~/.zshrc` not found** | Make sure Oh My Zsh is installed before running the Node/uv installers |
| **WSL paths** | Use `/home/username/...` not `/Users/username/...` (that's macOS) |
| **GitHub auth fails** | Run `/mcp` inside Claude Code and select **Authenticate** for GitHub |
| **Sentry auth fails** | Run `/mcp` inside Claude Code and select **Authenticate** for Sentry |
| **Clipboard not working** | Ensure `alias pbcopy='clip.exe'` is in `~/.zshrc` and restart your shell |
| **VS Code can't see WSL** | Install the **WSL** extension in VS Code on the Windows side |
