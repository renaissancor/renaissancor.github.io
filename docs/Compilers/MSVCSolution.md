# <Solution Name>

## Overview

This solution contains multiple MSVC projects built with Visual Studio 2022.
All projects in this solution follow a unified build and runtime configuration
to prevent Debug/Release inconsistencies and runtime incompatibilities.

- Toolchain: MSVC (Visual Studio 2022)
- Platform: Windows x64
- Language: C++ (MSVC)

---

## Build Configurations

| Configuration | Description |
|---------------|-------------|
| Debug         | Debugging, validation, and development builds |
| Release       | Optimized production build |

---

## Global Build Policy (Tier 1)

The following settings **must be identical across all projects** in this solution.

| Category | Setting |
|--------|---------|
| Platform Toolset | v143 |
| C++ Standard | C++20 (or project-specific) |
| C Standard | C17 |
| Warning Level | /W4 |
| Warnings as Errors | /WX |
| Runtime Library | /MD (Release), /MDd (Debug) |
| Exception Handling | /EHsc |
| RTTI | Enabled (/GR) |

> Any deviation from the above may cause link errors or undefined runtime behavior.

---

## Debug vs Release Policy

| Aspect | Debug | Release |
|------|-------|---------|
| Optimization | /Od | /O2 |
| Whole Program Optimization | Disabled | Enabled (/GL) |
| Incremental Linking | Enabled | Disabled |
| Runtime Checks | Enabled | Disabled |
| Debug Symbols | Enabled (/Zi) | Enabled (/Zi) |

---

## Release Build Checklist

- [ ] No Debug CRT linked in Release build
- [ ] No warnings at `/W4`
- [ ] `/GL` and LTCG enabled
- [ ] No logic dependent on `assert`
- [ ] No uninitialized variables
- [ ] All projects use the same Toolset

---

## Notes

- Mixing `/MT` and `/MD` across projects is prohibited.
- Debug-only behavior must not affect Release logic.
- All third-party libraries must match the solution CRT and toolset.

---

## Maintainers

- Owner: <Name / Team>
- Last Updated: <YYYY-MM-DD>
