This is a comprehensive guide to setting up a high-performance Claude terminal environment on macOS with GitHub integration, complex reasoning via Sequential Thinking, and the Serena agent.

---

# Ultimate Claude Terminal Setup Guide (macOS)

This guide configures **Claude Code** (the CLI) with three powerful Model Context Protocol (MCP) servers.

## 1. Prerequisites

First, install the core package managers and the Claude CLI.

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install core tools
brew install claude uv node

# Initialize Claude (follow the login prompts)
claude

```

---

## 2. MCP Server: Sequential Thinking

This server allows Claude to use a "Chain of Thought" process, breaking down complex problems into steps before executing.

**Installation:**

```bash
claude mcp add sequential-thinking -- npx -y @modelcontextprotocol/server-sequential-thinking

```

---

## 3. MCP Server: GitHub

Connects Claude to your repositories for PR management, issue tracking, and code search. 

### Step A: Generate a GitHub Token

1. Go to [GitHub Personal Access Tokens (Classic)](https://github.com/settings/tokens).
2. **Required Scopes:**

- `repo` (Full control of private repositories)
- `read:org` (Read org data)
- `user:email` (Identify user)
- `workflow` (Optional: for GitHub Actions)

### Step B: Configure Environment

Open your shell profile (e.g., `~/.zshrc` or `~/.bashrc`):

```bash
code ~/.zshrc
```

Add the following line (replace with your actual token):

```bash
export GITHUB_PERSONAL_ACCESS_TOKEN="ghp_your_token_here"

```

Apply the changes:

```bash
source ~/.zshrc

```

### Step C: Add to Claude

```bash
claude mcp add github -- npx -y @modelcontextprotocol/server-github
```

---

## 4. MCP Server: Serena

Serena is a professional coding agent. In a terminal environment, we must disable the web dashboard to prevent timeouts.

### Step A: Permanent Installation

Using `uvx` every time can be slow. Installing it as a tool ensures instant startup.

```bash
uv tool install git+https://github.com/oraios/serena
uv tool update-shell

```

### Step B: Headless Configuration

Create the configuration directory and file to disable the dashboard and set your project path.

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

Add Serena using the local tool path for maximum speed:

```bash
claude mcp add serena -- /Users/$(whoami)/.local/bin/serena start-mcp-server

```

---

## 5. Summary of `~/.claude.json`

Your final configuration file (located at `~/.claude.json`) should look similar to this structure:

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
      "command": "/Users/jaeheungpark/.local/bin/serena",
      "args": ["start-mcp-server"]
    }
  }
}
```

---

## 6. Verification

Start Claude and run the list command to ensure all status indicators are green:

```bash
claude
❯ /mcp list

```

### Troubleshooting

- **Timeout Errors:** Usually caused by Serena trying to open a browser. Ensure `web_dashboard: false` is set in `~/.serena/serena_config.yml`.
- **Path Errors:** Ensure your `projects` in Serena are simple strings (e.g., `"/Users/name/Project"`) and not maps (no `name:` or `path:` keys).

