# Windows Developer Setup Guide (From Zero)

This guide sets up a fresh Windows 11 machine for C++ development using WinGet, ending with
a working Claude Code environment ready for ImGui + DirectX game engine and IOCP WinSocket
server portfolios.

> All commands run in **PowerShell**. Right-click the Start menu → **Terminal (Admin)** for steps that require elevation.

> For Linux-side development on the same machine (Python/ML, anything POSIX), set up WSL2
> alongside this: **[WSL_dev_setup.md](./WSL_dev_setup.md)**. The AI-harness layer
> (plugins, Codex offload, Serena, skills) is documented in
> **[Harness_setup.md](./Harness_setup.md)** and applies here too.

---

## 1. Windows Update

Ensure the OS and drivers are fully up to date before installing anything.

```
Settings → Windows Update → Check for updates → Restart if prompted
```

---

## 2. WinGet

WinGet ships with Windows 11 by default. Verify it is available and up to date.

```powershell
# Verify WinGet is installed
winget --version

# Update WinGet itself via the App Installer
winget upgrade Microsoft.AppInstaller
```

> On **Windows 10**: open the Microsoft Store → search **App Installer** → Install/Update.

---

## 3. Windows Terminal + PowerShell 7

The built-in `conhost.exe` is outdated. Install the modern terminal and the latest PowerShell.

```powershell
winget install Microsoft.WindowsTerminal
winget install Microsoft.PowerShell
```

> Close the old terminal. From now on, open **Windows Terminal** and switch the default profile to **PowerShell** (the new v7+, not the old Windows PowerShell 5.x).

### Allow Script Execution

```powershell
# Required for install scripts and shell profiles
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

---

## 4. Oh My Posh (Shell Prompt)

Oh My Posh is the Windows equivalent of Oh My Zsh — adds Git branch display and a clean prompt.

```powershell
winget install JanDeDobbeleer.OhMyPosh

# Install a Nerd Font (required for icons)
oh-my-posh font install meslo
```

Set your Windows Terminal font to **MesloLGM Nerd Font** in Settings → Profiles → PowerShell → Appearance.

```powershell
# Add Oh My Posh to your PowerShell profile
if (!(Test-Path $PROFILE)) { New-Item $PROFILE -Force }
Add-Content $PROFILE "`noh-my-posh init pwsh --config `"`$env:POSH_THEMES_PATH\robbyrussell.omp.json`" | Invoke-Expression"

# Apply immediately
. $PROFILE
```

---

## 5. Git

```powershell
winget install Git.Git

# Restart terminal so git is on PATH, then configure:
git config --global user.name "Your Name"
git config --global user.email "your@email.com"
git config --global core.editor "code --wait"
git config --global init.defaultBranch main
```

### SSH Key for GitHub

```powershell
# Generate key (use your GitHub email)
ssh-keygen -t ed25519 -C "your@email.com"
# Press Enter three times to accept defaults

# Copy public key to clipboard
Get-Content "$env:USERPROFILE\.ssh\id_ed25519.pub" | Set-Clipboard
```

Go to [GitHub → Settings → SSH Keys → New SSH Key](https://github.com/settings/keys), paste, and save.

```powershell
# Test the connection
ssh -T git@github.com
# Expected: Hi username! You've successfully authenticated...
```

---

## 6. Visual Studio 2022 Community (C++ Workload)

> This is the core of your C++ dev environment. The `NativeDesktop` workload includes MSVC, the Windows SDK, and CMake tools — everything needed for DirectX and WinSocket.

```powershell
# Install Visual Studio 2022 Community with C++ Desktop workload
winget install Microsoft.VisualStudio.2022.Community --override "--add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended --passive"
```

This installs:
- MSVC v143 compiler toolchain
- Windows 11 SDK (includes DirectX, WinSock2, IOCP headers)
- CMake tools for Windows
- MSBuild

> Installation takes 10-20 minutes depending on your connection speed.

---

## 7. CMake

Visual Studio includes CMake tools, but install the standalone binary so `cmake` is available in the terminal.

```powershell
winget install Kitware.CMake

# Restart terminal, then verify
cmake --version
```

---

## 8. vcpkg (C++ Package Manager)

vcpkg manages C++ libraries. Clone it to your user folder so it persists across projects.

```powershell
# Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git "$env:USERPROFILE\vcpkg"

# Bootstrap (compiles the vcpkg binary)
& "$env:USERPROFILE\vcpkg\bootstrap-vcpkg.bat"

# Add vcpkg to your PATH permanently
[Environment]::SetEnvironmentVariable("VCPKG_ROOT", "$env:USERPROFILE\vcpkg", "User")
$currentPath = [Environment]::GetEnvironmentVariable("PATH", "User")
[Environment]::SetEnvironmentVariable("PATH", "$currentPath;$env:USERPROFILE\vcpkg", "User")

# Restart terminal, then integrate with Visual Studio (run once)
vcpkg integrate install
```

---

## 9. C++ Portfolio Libraries

### ImGui + DirectX (Game Engine Portfolio)

```powershell
# DirectX Tool Kit (DX11 and DX12)
vcpkg install directxtk:x64-windows
vcpkg install directxtk12:x64-windows

# DirectX texture and mesh utilities
vcpkg install directxtex:x64-windows
vcpkg install directxmesh:x64-windows

# ImGui with DirectX 11, DirectX 12, and Win32 bindings
vcpkg install "imgui[dx11-binding,dx12-binding,win32-binding]":x64-windows
```

### IOCP WinSocket Server Portfolio

> IOCP and WinSocket are part of the **Windows SDK** — no vcpkg packages needed. Just include the headers and link the libraries in your project.

```cpp
// Required headers
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>    // AcceptEx, GetAcceptExSockAddrs

// Required linker dependencies
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
```

```powershell
# Verify Windows SDK headers are present
Test-Path "C:\Program Files (x86)\Windows Kits\10\Include"
```

---

## 10. Node.js (via nvm-windows)

> **Do NOT** use `winget install OpenJS.NodeJS` directly — use nvm-windows to switch Node versions per project.

```powershell
winget install CoreyButler.NVMforWindows

# Restart terminal, then install LTS
nvm install lts
nvm use lts

# Verify (v24+ LTS as of 2026)
node -v
```

---

## 11. Python + uv

```powershell
winget install astral-sh.uv

# uv manages Python interpreters itself — no separate Python install needed
uv python install 3.13

# Restart terminal, then verify
uv --version
uv run python --version
```

---

## 12. GitHub CLI

```powershell
winget install GitHub.cli

# Authenticate
gh auth login
# Choose: GitHub.com → SSH → Login with a web browser
```

> `gh` is also why no GitHub MCP server is configured anymore — Claude Code drives `gh`
> natively for PRs, issues, and API calls.

---

## 13. VS Code

```powershell
winget install Microsoft.VisualStudioCode

# Restart terminal — 'code' command is now on PATH
code --version
```

---

## 14. Verify All Installations

```powershell
git --version && node -v && uv --version && gh --version && cmake --version && vcpkg version
```

---

## 15. Claude Code

Claude Code runs natively on Windows (PowerShell) — Git for Windows (Section 5) is a
prerequisite. For heavy non-C++ work I run it inside WSL instead (see
[WSL_dev_setup.md](./WSL_dev_setup.md)); native Windows is the right choice for the
Visual Studio / DirectX portfolio work here.

```powershell
# Native installer
irm https://claude.ai/install.ps1 | iex

# Fallback: npm-based install
npm install -g @anthropic-ai/claude-code

# First run — follow the login prompts
claude
```

Log in with a **Claude account** (Pro/Max/Team/Enterprise) unless you specifically want
API-metered billing.

### Harness layer

Plugins (oh-my-claudecode, codex), the Codex CLI, skills, the CLAUDE.md hierarchy, and
`settings.json` permissions are documented in
**[Harness_setup.md](./Harness_setup.md)** — the `/plugin` flow is identical.
Windows-specific bits:

```powershell
# Codex CLI (no Homebrew here) — npm install, ChatGPT login
npm install -g @openai/codex
codex login
```

### Serena MCP (per-project)

As on the other platforms, register Serena per project in a committed `.mcp.json` at the
repo root (uv from Section 11 provides `uvx`):

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

```powershell
# Verify
claude mcp list
```

> **Dropped since the February version of this guide:** Sequential Thinking MCP (built-in
> extended thinking covers it), Sentry MCP (never used), GitHub MCP (`gh` does it better),
> and the global uv-tool Serena install (per-project `uvx` keeps repos self-contained).

---

## 16. Verification

```powershell
claude --version && codex --version
claude
❯ /mcp        # MCP servers green
❯ /plugin     # plugins enabled
```

### Troubleshooting

| Issue | Solution |
|---|---|
| **Serena timeout** | Use `--context ide-assistant` in the `.mcp.json` args; use forward slashes in any paths |
| **`winget` not found** | Update App Installer from the Microsoft Store |
| **`nvm` not found** | Restart terminal after `winget install CoreyButler.NVMforWindows` |
| **`vcpkg` not found** | Restart terminal after setting the PATH environment variable |
| **VS workload missing** | Open Visual Studio Installer → Modify → add **Desktop development with C++** |
| **DirectX headers missing** | Ensure Windows 11 SDK is checked in the VS workload |
| **Script execution blocked** | Run `Set-ExecutionPolicy RemoteSigned -Scope CurrentUser` |
| **`claude` install fails natively** | Ensure Git for Windows is installed, or use the npm fallback |

---

## Windows vs macOS vs WSL — Key Differences

| | Windows | macOS | WSL |
|---|---|---|---|
| Package manager | `winget` | `brew` | `apt` |
| Terminal | Windows Terminal + PS7 | iTerm2 + zsh | Windows Terminal + zsh |
| Shell profile | `$PROFILE` | `~/.zshrc` + `~/.zprofile` | `~/.zshrc` |
| Copy to clipboard | `Set-Clipboard` | `pbcopy` | `clip.exe` |
| Open folder in GUI | `explorer.exe .` | `open .` | `explorer.exe .` |
| C++ package manager | vcpkg | vcpkg / brew | vcpkg / apt |
| DirectX / IOCP | Native (Windows SDK) | Not available | Not available |

---

## Checklist

- [ ] Windows fully updated
- [ ] Windows Terminal installed, PowerShell 7 set as default profile
- [ ] Script execution policy set to `RemoteSigned`
- [ ] Oh My Posh installed with Nerd Font
- [ ] Git configured with name, email, and SSH key added to GitHub
- [ ] Visual Studio 2022 Community installed with **Desktop development with C++** workload
- [ ] `cmake` available in terminal (`cmake --version`)
- [ ] vcpkg cloned, bootstrapped, and integrated (`vcpkg integrate install`)
- [ ] DirectX and ImGui packages installed via vcpkg
- [ ] `node` installed via nvm-windows (v24+ LTS)
- [ ] `uv` installed, Python via `uv python install`
- [ ] GitHub CLI installed and authenticated (`gh auth status`)
- [ ] VS Code installed (`code --version`)
- [ ] Claude Code installed and logged in; harness per [Harness_setup.md](./Harness_setup.md)

---

_Last updated: August 2026 — Claude layer modernized (native installer, plugins, per-project
Serena; Sequential Thinking / Sentry / GitHub MCP removed), Python now managed by uv._
