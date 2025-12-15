# <Project Name>

## Project Type

- Type: Application (.exe) / Static Library (.lib) / Dynamic Library (.dll)
- Language: C++
- Platform: Windows x64

---

## Build Dependencies

### Internal Dependencies
- <ProjectA>
- <ProjectB>

### External Dependencies
- <LibraryName> (vX.Y)
- Build via: vcpkg / prebuilt binaries / source

---

## Build Configuration

| Setting | Value |
|-------|-------|
| Platform Toolset | v143 |
| C++ Standard | C++20 |
| Runtime Library | /MD (Release), /MDd (Debug) |
| Warning Level | /W4 |
| Warnings as Errors | Enabled |
| Exception Handling | /EHsc |
| RTTI | Enabled |

---

## Compiler Options (Key)

| Option | Value | Notes |
|------|-------|-------|
| Optimization | /Od (Debug), /O2 (Release) | |
| Debug Info | /Zi | |
| Whole Program Optimization | /GL (Release) | |
| Runtime Checks | Debug only | |

---

## Linker Settings

| Setting | Value |
|-------|-------|
| Incremental Linking | Debug only |
| Additional Libraries | <list if any> |
| Output | $(OutDir)/<binary name> |

---

## Preprocessor Definitions

| Configuration | Macros |
|--------------|--------|
| Debug | `_DEBUG` |
| Release | `NDEBUG` |

---

## Common Pitfalls

- Do not mix Debug and Release libraries
- Do not rely on `assert` for runtime logic
- Avoid undefined behavior masked by Debug CRT

---

## How to Build

1. Open `<Solution>.sln` in Visual Studio 2022
2. Select configuration: Debug or Release
3. Build the project

---

## Maintainers

- Owner: <Name>
- Last Updated: <YYYY-MM-DD>
