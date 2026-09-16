# allyouneed_bongocat

Windows desktop application built with C++20 and the Win32 API.

## Development

See [CONTRIBUTING.md](CONTRIBUTING.md) for pull request and English commit
message conventions.

### Windows

Requirements:

- Windows 10 or later
- Visual Studio 2026 with the **Desktop development with C++** workload
- Windows 10/11 SDK

Build from a Developer PowerShell:

```powershell
msbuild allyouneed_bongocat.slnx /m /restore /p:Configuration=Debug /p:Platform=x64
```

Pull requests run Debug and Release x64 builds. The Release build also runs
Microsoft C++ Code Analysis and C++ Core Check.

### macOS

The current application uses Win32 APIs and cannot be built on macOS. New
platform-independent behavior should be isolated from the Win32 entry point so
it can be tested and reused by a future native macOS target. A macOS CI build
must be added when an Xcode, CMake, or other macOS target is introduced.

## Repository safeguards

- Changes to `master` must go through a pull request.
- CI, dependency review, CodeQL, resolved review threads, and an approving
  review are required before squash merge.
- Copilot code review is requested automatically for new pull requests and
  every subsequent push.
- Dependabot checks GitHub Actions versions weekly.
- Secret scanning and push protection are enabled.
