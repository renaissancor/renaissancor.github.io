# Windows Developer Setup Guide (From Zero)

This guide sets up a fresh Windows 11 machine for C++ development using WinGet, ending with a working Claude Code environment ready for ImGui + DirectX game engine and IOCP WinSocket server portfolios.

> All commands run in **PowerShell**. Right-click the Start menu → **Terminal (Admin)** for steps that require elevation.

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

> Installation takes 10–20 minutes depending on your connection speed.

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

# Verify (should be v22+)
node -v
```

---

## 11. Python + uv

```powershell
winget install Python.Python.3.12
winget install astral-sh.uv

# Restart terminal, then verify
python --version
uv --version
```

---

## 12. GitHub CLI

```powershell
winget install GitHub.cli

# Authenticate
gh auth login
# Choose: GitHub.com → SSH → Login with a web browser
```

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
git --version && node -v && python --version && uv --version && gh --version && cmake --version && vcpkg version
```

---

## 15. Claude Code

```powershell
npm install -g @anthropic-ai/claude-code
claude
```

Select your login method when prompted:

1. **Claude account** — Pro, Max, Team, or Enterprise
2. **Anthropic Console** — API usage billing
3. **3rd-party platform** — Amazon Bedrock, Microsoft Foundry, or Vertex AI

---

## 16. MCP Server: Sequential Thinking

Enables Claude to break down complex problems into step-by-step reasoning.

```powershell
claude mcp add sequential-thinking -- npx -y @modelcontextprotocol/server-sequential-thinking
```

---

## 17. MCP Server: GitHub

Connects Claude to your repositories for PR management, issue tracking, and code search. Uses GitHub's official hosted endpoint with OAuth — no personal access token required.

### Step A: Add to Claude

```powershell
claude mcp add --transport http github https://api.githubcopilot.com/mcp/
```

### Step B: Authenticate

```powershell
# Inside Claude Code, open the MCP menu and follow the browser login flow
claude
❯ /mcp
```

Select **github** → **Authenticate** and complete the GitHub OAuth flow in your browser.

---

## 18. MCP Server: Serena

Serena is a professional coding agent. The web dashboard must be disabled in terminal environments to prevent timeouts.

### Step A: Install Permanently

```powershell
uv tool install git+https://github.com/oraios/serena
uv tool update-shell
. $PROFILE
```

### Step B: Headless Configuration

```powershell
New-Item -Path "$env:USERPROFILE\.serena" -ItemType Directory -Force

@"
web_dashboard: false
web_dashboard_open_on_launch: false
projects:
  - "C:/Users/$env:USERNAME/Project"
"@ | Out-File "$env:USERPROFILE\.serena\serena_config.yml" -Encoding UTF8
```

### Step C: Add to Claude

```powershell
# Use the local tool path for maximum startup speed
claude mcp add serena -- "$env:USERPROFILE\.local\bin\serena.exe" start-mcp-server
```

---

## 19. Expected `~/.claude.json`

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
    "serena": {
      "command": "C:\\Users\\your_username\\.local\\bin\\serena.exe",
      "args": ["start-mcp-server"]
    }
  }
}
```

---

## 20. Verification

```powershell
claude
❯ /mcp list
```

All status indicators should be **green**.

### Troubleshooting

| Issue | Solution |
|---|---|
| **Serena timeout** | Ensure `web_dashboard: false` in `%USERPROFILE%\.serena\serena_config.yml` |
| **Path errors (Serena)** | Use forward slashes in `projects` (e.g., `"C:/Users/name/Project"`) |
| **`winget` not found** | Update App Installer from the Microsoft Store |
| **`nvm` not found** | Restart terminal after `winget install CoreyButler.NVMforWindows` |
| **`vcpkg` not found** | Restart terminal after setting the PATH environment variable |
| **VS workload missing** | Open Visual Studio Installer → Modify → add **Desktop development with C++** |
| **DirectX headers missing** | Ensure Windows 11 SDK is checked in the VS workload |
| **Script execution blocked** | Run `Set-ExecutionPolicy RemoteSigned -Scope CurrentUser` |

---

## Windows vs macOS vs Linux — Key Differences

| | Windows | macOS | Linux |
|---|---|---|---|
| Package manager | `winget` | `brew` | `apt` / `dnf` |
| Terminal | Windows Terminal + PS7 | iTerm2 + zsh | GNOME Terminal + zsh |
| Shell profile | `$PROFILE` | `~/.zshrc` | `~/.zshrc` |
| Copy to clipboard | `Set-Clipboard` | `pbcopy` | `xclip` |
| Open folder in GUI | `explorer.exe .` | `open .` | `xdg-open .` |
| C++ package manager | vcpkg | vcpkg / brew | vcpkg / apt |
| DirectX | Native (Windows SDK) | Not available | Not available |
| IOCP | Native (Windows SDK) | Not available | Not available |

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
- [ ] `node` installed via nvm-windows (v22+)
- [ ] `python`, `uv` installed
- [ ] GitHub CLI installed and authenticated (`gh auth status`)
- [ ] VS Code installed (`code --version`)
- [ ] Claude Code installed and logged in (`claude`)
- [ ] All three MCP servers green (`/mcp list`)
