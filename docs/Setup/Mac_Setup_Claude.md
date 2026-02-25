# Claude Code Setup Guide (macOS)

This guide configures a high-performance **Claude Code** terminal environment on macOS with GitHub integration, Sequential Thinking, and the Serena agent.

---

## 1. Prerequisites

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Add Homebrew to PATH (run the two lines the installer prints, then:)
source ~/.zprofile

# Install core formulae (already done if you followed MacBook_Dev_Setup.md)
brew install uv node

# Install the Claude desktop app (GUI)
brew install --cask claude

# Install Claude Code CLI (separate native binary, auto-updates)
curl -fsSL https://claude.ai/install.sh | bash
```

> **Claude desktop app vs Claude Code CLI**
> - `brew install --cask claude` installs **Claude.app** — the GUI desktop application.
> - The **Claude Code CLI** (`claude` terminal command) is a separate native binary installed via the curl script above. Verify with `claude --version`.

```bash
# Initialize Claude Code CLI (follow the login prompts)
claude
```

Select your login method when prompted:

1. **Claude account** — Pro, Max, Team, or Enterprise
2. **Anthropic Console** — API usage billing
3. **3rd-party platform** — Amazon Bedrock, Microsoft Foundry, or Vertex AI

---

## 2. MCP Server: Sequential Thinking (Optional)

> **Note:** Claude's built-in extended thinking (`/think` during chat) covers most reasoning use cases natively. Add this server only if you want explicit step-by-step tool calls in your workflow.

```bash
claude mcp add sequential-thinking -- npx -y @modelcontextprotocol/server-sequential-thinking
```

---

## 3. MCP Server: GitHub

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

## 4. MCP Server: Sentry

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

## 5. MCP Server: Serena

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

## 6. Project Memory (CLAUDE.md)

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

## 7. Permissions & Settings

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

## 8. Expected `~/.claude.json`

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
      "command": "/Users/your_username/.local/bin/serena",
      "args": ["start-mcp-server"]
    }
  }
}
```

---

## 9. Verification

```bash
claude
❯ /mcp
```

All status indicators should be **green**.

---

## Troubleshooting

| Issue | Solution |
|---|---|
| **Serena timeout** | Ensure `web_dashboard: false` in `~/.serena/serena_config.yml` |
| **Path errors (Serena)** | Use simple strings in `projects` (e.g., `"/Users/name/Project"`), not maps with `name:` or `path:` keys |
| **Node not found** | Run `brew install node` or restart your shell |
| **Homebrew not on PATH** | Run `source ~/.zprofile` or restart your terminal |
| **GitHub auth fails** | Run `/mcp` inside Claude Code and select **Authenticate** for GitHub |
| **Sentry auth fails** | Run `/mcp` inside Claude Code and select **Authenticate** for Sentry |
