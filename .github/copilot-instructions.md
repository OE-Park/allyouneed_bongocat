# Repository instructions

- Treat warnings as errors and preserve C++20 compatibility.
- Write commit messages in English with a concise imperative subject.
- Keep platform-independent behavior separate from Win32 UI and lifecycle code.
- Check return values from Win32 APIs and propagate or report failures explicitly.
- Prefer RAII wrappers for owned handles and resources.
- Do not commit generated binaries, certificates, signing identities, provisioning profiles, or secrets.
- Require tests for extracted platform-independent behavior.
- During pull request review, prioritize resource leaks, unsafe casts, integer truncation, unchecked API failures, UI-thread blocking, and Windows/macOS portability regressions.
