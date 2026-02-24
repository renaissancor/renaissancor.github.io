# Ultimate Claude Terminal Setup Guide (macOS)

This guide configures a high-performance **Claude Code** terminal environment on macOS with GitHub integration, Sequential Thinking, and the Serena agent.

---

## 1. Prerequisites

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Add Homebrew to PATH (run the two lines the installer prints, then:)
source ~/.zprofile

# Install core tools
brew install claude uv node

# Initialize Claude (follow the login prompts)
claude
```

Select your login method when prompted:

1. **Claude account** — Pro, Max, Team, or Enterprise
2. **Anthropic Console** — API usage billing
3. **3rd-party platform** — Amazon Bedrock, Microsoft Foundry, or Vertex AI

---

## 2. MCP Server: Sequential Thinking

Enables Claude to break down complex problems into step-by-step reasoning.

```bash
claude mcp add sequential-thinking -- npx -y @modelcontextprotocol/server-sequential-thinking
```

---

## 3. MCP Server: GitHub

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

## 4. MCP Server: Serena

Serena is a professional coding agent. The web dashboard must be disabled in terminal environments to prevent timeouts.

### Step A: Install Permanently

> Using `uvx` every time can be slow. Installing as a tool ensures instant startup.

```bash
uv tool install git+https://github.com/oraios/serena
uv tool update-shell
source ~/.zshrc
```

### Step B: Headless Configuration

```bash
mkdir -p ~/.serena

cat <<EOF > ~/.serena/serena_config.yml
web_dashboard: false
web_dashboard_open_on_launch: false
projects:
  - "/Users/$(whoami)/Project"
EOF
```

### Step C: Add to Claude

```bash
# Use the local tool path for maximum startup speed
claude mcp add serena -- /Users/$(whoami)/.local/bin/serena start-mcp-server
```

---

## 5. Expected `~/.claude.json`

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
      "command": "/Users/your_username/.local/bin/serena",
      "args": ["start-mcp-server"]
    }
  }
}
```

---

## 6. Verification

```bash
claude
❯ /mcp list
```

All status indicators should be **green**.

### Troubleshooting

| Issue | Solution |
|---|---|
| **Serena timeout** | Ensure `web_dashboard: false` in `~/.serena/serena_config.yml` |
| **Path errors (Serena)** | Use simple strings in `projects` (e.g., `"/Users/name/Project"`), not maps with `name:` or `path:` keys |
| **Node not found** | Run `brew install node` or restart your shell |
| **Homebrew not on PATH** | Run `source ~/.zprofile` or restart your terminal |
