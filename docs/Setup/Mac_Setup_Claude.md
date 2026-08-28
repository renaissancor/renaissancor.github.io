# Claude Code + AI Harness Setup (macOS)

This guide rebuilds my full AI coding environment on macOS as of August 2026. It goes well
beyond a bare Claude Code install: the working setup is a **harness** — Claude Code at the
center, extended by the **oh-my-claudecode** plugin (multi-agent orchestration), the
**Codex CLI** as a token-offload second engine, the **rtk** proxy for token-efficient shell
output, **Serena** for semantic code navigation, and **skills** for repeatable workflows.

> The February 2026 version of this guide described four MCP servers (Sequential Thinking,
> GitHub, Sentry, Serena) on a stock install. Sequential Thinking is superseded by built-in
> extended thinking, and hosted connectors (GitHub, Slack, Notion, Google Drive…) are now
> managed from the Claude app side rather than hand-added MCP endpoints. This rewrite
> documents what I actually run.

---

## 1. Claude Code CLI

```bash
# Prerequisites (see MacBook_Dev_Setup.md): brew, uv, node

# Claude desktop app (GUI) — optional but useful
brew install --cask claude

# Claude Code CLI — native binary, self-updating
curl -fsSL https://claude.ai/install.sh | bash

# Verify — installs to ~/.local/bin/claude
claude --version
```

> **Desktop app vs CLI:** `brew install --cask claude` installs **Claude.app** (GUI).
> The **Claude Code CLI** (`claude` in the terminal) is a separate native binary from the
> curl script. They share your Claude account but update independently.

```bash
# First run — follow the login prompts
claude
```

Log in with a **Claude account** (Pro/Max/Team/Enterprise) unless you specifically want
API-metered billing via Anthropic Console.

---

## 2. Plugins

Plugins are the biggest change since early 2026 — they bundle agents, skills, hooks, and
MCP servers into installable units. Manage them with `/plugin` inside Claude Code.

My enabled set:

| Plugin | Marketplace | What it adds |
|---|---|---|
| **oh-my-claudecode** | `omc` | Multi-agent orchestration: specialized agents (executor, architect, verifier…), execution modes (autopilot, ralph, ultrawork), team pipelines, HUD statusline |
| **codex** | `openai-codex` | Bridges the Codex CLI as a rescue/second-opinion subagent |
| **warp** | `claude-code-warp` | Warp terminal integration |

Inside Claude Code:

```
/plugin
# → Browse marketplaces → add the marketplace → install → enable
```

oh-my-claudecode also has a guided setup — after installing, just say **"setup omc"** in a
session and it configures its hooks, HUD, and state directories itself.

---

## 3. Codex CLI (Second Engine)

The Codex CLI runs OpenAI models with its own context window. I use it as a **token
offload**: bulk file reading, self-contained lookups, and second opinions run in Codex so
the file contents never enter Claude's context — only the conclusion comes back.

```bash
# The codex cask bundles the CLI
brew install --cask codex

# Log in (ChatGPT account — no API key needed)
codex login

# Verify
codex --version
```

The core pattern — read-only, self-contained prompts whose return value is the answer:

```bash
codex exec --sandbox read-only "Summarize what this repo's scripts/ directory does. Reply with one line per script."
```

Mark trusted repos in `~/.codex/config.toml` so `codex exec` runs there without prompts.
Rules I follow: Codex never touches secrets, and anything needing conversation context gets
that context paraphrased into the prompt (Codex starts blank every call).

---

## 4. rtk (Token Proxy)

rtk ("Rust Token Killer") wraps common CLI commands (`git
status`, `ls`, test runners…) and returns compressed, token-efficient output — 60–90%
savings on routine dev operations. A Claude Code **PreToolUse hook** rewrites commands to
pass through rtk transparently.

```bash
brew install rtk

# Verify — should print savings analytics, not "command not found"
rtk gain
```

> **Name collision:** if `rtk gain` fails, you may have installed a different `rtk`
> (Rust Type Kit). Check `which rtk` and the formula source.

The hook lives in `~/.claude/settings.json` under `hooks.PreToolUse`; oh-my-claudecode's
setup wires it. Useful meta-commands: `rtk gain --history` (per-command savings),
`rtk discover` (finds missed opportunities in your Claude Code history).

---

## 5. MCP Servers

My current MCP philosophy: **fewer, scoped, project-level where possible.** Hosted
integrations (Slack, Notion, Google Drive, Gmail…) come through Claude-app connectors
automatically; hand-configured MCP servers are only for things connectors don't cover.

### Serena (semantic code navigation) — per-project

Serena provides LSP-backed symbol search, references, and precise edits. I register it
**per project** in a committed `.mcp.json` rather than globally — projects that don't need
it don't pay its startup cost:

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

> `--context ide-assistant` trims Serena's toolset to what makes sense inside Claude Code.
> The old advice to `uv tool install` it globally and disable the web dashboard still works
> (`web_dashboard: false` in `~/.serena/serena_config.yml`), but per-project `uvx` keeps
> machines reproducible from the repo alone.

### Google Docs / Sheets — user-level

Local stdio servers (run via `uv`/`uvx`) for reading and editing Google Docs/Sheets from
sessions. Added with `claude mcp add`; check status any time:

```bash
claude mcp list
```

### What I dropped since February

- **Sequential Thinking** — built-in extended thinking covers it; the extra server was noise.
- **Sentry** — never used it; it came from a tutorial, not a need.
- **GitHub MCP** — the `gh` CLI does everything (PRs, issues, API) with less overhead, and
  Claude Code drives `gh` natively.

---

## 6. Skills

Skills are reusable instruction sets invoked as `/name` or auto-triggered. Personal skills
live in `~/.claude/skills/`; I keep the sources in the repos they belong to and **symlink**
them in, so the skill is versioned with the project it serves:

```bash
ln -s ~/my-project/skills/my-skill ~/.claude/skills/my-skill
```

My daily-driver example: a two-phase daily-report system —

- `daily-report-capture` — at the end of any work session, writes a digest of that
  session's work into an inbox directory (fast, no report composition)
- `daily-report` — composes the day's digests into a bilingual report pair, lints, commits

The compose step runs unattended at 21:30 via **launchd**:

```bash
# ~/Library/LaunchAgents/com.<user>.daily-report-compose.plist
launchctl list | grep daily-report
```

This pattern — capture cheaply during sessions, compose on a schedule — generalizes to any
"end of day roll-up" workflow.

---

## 7. Memory: CLAUDE.md Hierarchy

Claude Code reads instruction files at session start, most-specific last:

| File | Scope | What goes in it |
|---|---|---|
| `~/.claude/CLAUDE.md` | All projects | Cross-project rules: delegation policy, secret-handling rules, server-protection rules. Can `@include` other files (mine pulls in `RTK.md` and `CODEX.md`) |
| `<repo>/CLAUDE.md` | One project, committed | Build/test commands, conventions, hard operational rules for that repo |
| Auto-memory | Per project, private | Claude's own persistent notes across sessions |

```bash
# Generate a starter project CLAUDE.md from inside Claude Code:
❯ /init
```

Keep them concise: delete anything Claude can infer from the code itself. The highest-value
content is **rules that prevent damage** (what never to run, what never to read) and
**decisions that aren't visible in the code**.

---

## 8. Permissions & Settings

`~/.claude/settings.json` controls permissions, hooks, model choice, and the statusline.
The shape of mine:

```json
{
  "model": "opus",
  "permissions": {
    "allow": ["Bash(git *)", "Bash(uv run *)", "..."],
    "deny": ["Bash(sudo *)", "Read(.env*)", "..."]
  },
  "hooks": {
    "PreToolUse": ["... rtk rewrite + guard hooks ..."]
  },
  "statusLine": { "command": "node $HOME/.claude/hud/omc-hud.mjs" }
}
```

- **Deny-list reads of secret files** (`.env*`, key files) — the deny rule is cheap
  insurance against a careless session.
- Project-level `.claude/settings.json` (committed) sets team defaults; user-level wins.
- The statusline HUD comes from oh-my-claudecode and shows model, context usage, and
  active agents.

---

## 9. Verification

```bash
# CLI + engines
claude --version && codex --version && rtk gain

# MCP servers all green
claude mcp list

# Plugins enabled
claude
❯ /plugin
```

---

## Troubleshooting

| Issue | Solution |
|---|---|
| **Serena timeout** | Use `--context ide-assistant`; if using a global config, ensure `web_dashboard: false` in `~/.serena/serena_config.yml` |
| **`rtk gain` fails** | Wrong `rtk` binary installed (name collision) — check `which rtk` |
| **Codex asks for approval constantly** | Add the repo to the trusted list in `~/.codex/config.toml`, or pass `--sandbox read-only` |
| **`claude` not found** | The native installer puts it in `~/.local/bin` — ensure that's on PATH |
| **Homebrew not on PATH** | `source ~/.zprofile` or restart the terminal |
| **Plugin hooks not firing** | Re-run the plugin's setup (e.g. "setup omc") after Claude Code updates |

---

_Last updated: August 2026 — rewritten from the stock-install guide to document the full
working harness (plugins, Codex offload, rtk, per-project Serena, skills, launchd automation)._
