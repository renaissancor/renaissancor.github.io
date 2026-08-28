# iTerm2 Setup Guide (macOS)

iTerm2 is the terminal you reach for when you need deep shell integration, persistent sessions, and fine-grained control. This guide covers configuration from scratch after installing via `brew install --cask iterm2`.

---

## 1. Shell Integration

Shell Integration is iTerm2's flagship feature. It marks each command in the scrollback buffer, tracks working directories per-session, and enables the toolbelt.

### Install

Open iTerm2, then either:

**Option A — Menu (recommended):**
`iTerm2 menu → Install Shell Integration`

**Option B — curl:**
```bash
curl -L https://iterm2.com/shell_integration/install_shell_integration.sh | bash
source ~/.zshrc
```

Shell Integration injects a small snippet into `~/.iterm2_shell_integration.zsh` and sources it from your `~/.zshrc`. After restarting iTerm2 you'll see small blue triangle markers on the left of each prompt — those are command marks.

---

## 2. Suppress the Login Banner

```bash
touch ~/.hushlogin
```

Removes the `Last login: ...` line that prints at the top of every new tab or window.

---

## 3. Profile: Text

**Preferences → Profiles → Default → Text**

| Setting | Value |
|---|---|
| Font | Fira Code (or any Nerd Font for icon support) |
| Size | 14–15 pt |
| Use ligatures | ✅ On (if using Fira Code) |
| Anti-aliased | ✅ On |

> To install Fira Code if not already present:
> ```bash
> brew install --cask font-fira-code
> ```

---

## 4. Profile: Window

**Preferences → Profiles → Default → Window**

| Setting | Value |
|---|---|
| Transparency | 10–15% (subtle depth) |
| Blur | ✅ On |
| Columns / Rows | 220 × 50 (adjust to your monitor) |

---

## 5. Profile: Terminal

**Preferences → Profiles → Default → Terminal**

| Setting | Value |
|---|---|
| Scrollback lines | ✅ Unlimited scrollback |
| Save lines to scrollback when an app status bar is present | ✅ On |

---

## 6. Profile: Keys — Natural Text Editing

By default, `Option+Left/Right` doesn't jump by word in iTerm2. Fix this by loading the Natural Text Editing preset:

**Preferences → Profiles → Default → Keys → Key Mappings → Presets → Natural Text Editing**

This makes `Option+Left/Right` (word jump), `Cmd+Left/Right` (line start/end), and `Option+Backspace` (delete word) behave like every other macOS text field.

---

## 7. Profile: Working Directory

Keep each new tab/split in the same directory as the current one rather than defaulting to `~`:

**Preferences → Profiles → Default → General → Working Directory → Reuse previous session's directory**

---

## 8. Status Bar

The status bar shows live context (directory, git branch, CPU, memory) at the bottom of each session.

**Preferences → Profiles → Default → Session → Status bar enabled ✅**

Click **Configure Status Bar** and drag in the components you want. A useful baseline:

| Component | Purpose |
|---|---|
| Current directory | Shows full path |
| Git state | Branch name + dirty indicator |
| CPU utilization | Spot runaway processes |
| Memory utilization | Keep an eye on heavy builds |
| Clock | Time at a glance |

---

## 9. Hotkey Window

A hotkey window drops iTerm2 over any application with a single keystroke — useful for quick commands without leaving your current context.

**Preferences → Keys → Hotkey → Create a Dedicated Hotkey Window**

Recommended binding: `Cmd+`` ` (backtick) or `Cmd+Shift+.`

Set the hotkey window profile to:
- **Transparency:** 20–25%
- **Style:** Full-Width Bottom of Screen or Top of Screen

---

## 10. General: Closing

**Preferences → General → Closing**

| Setting | Value |
|---|---|
| Confirm closing multiple sessions | ✅ On |
| Confirm "Quit iTerm2" | ✅ On |

Prevents accidental `Cmd+Q` from killing a running process.

---

## 11. Useful Shortcuts

| Shortcut | Action |
|---|---|
| `Cmd+D` | Split pane vertically |
| `Cmd+Shift+D` | Split pane horizontally |
| `Cmd+[` / `Cmd+]` | Move between panes |
| `Cmd+T` | New tab |
| `Cmd+Shift+H` | Open paste history |
| `Cmd+F` | Find in scrollback |
| `Cmd+Shift+E` | Reveal command marks (timestamped) |
| `Cmd+Click` on a path | Open file in editor |
| `Cmd+Click` on a URL | Open in browser |

---

## Checklist

- [ ] Shell Integration installed (blue triangle markers visible at prompt)
- [ ] `.hushlogin` created (no "Last login" banner)
- [ ] Font set to Fira Code with ligatures
- [ ] Unlimited scrollback enabled
- [ ] Natural Text Editing preset applied
- [ ] Working directory set to reuse previous session
- [ ] Status bar enabled with directory + git state
- [ ] Hotkey window configured (optional)
- [ ] Closing confirmations enabled
