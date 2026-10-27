# macos — Xcode 프로젝트 자리 (나중)

## 만들 때 (Mac에서)
- Xcode > New Project > macOS > App
  - Product Name: `BongoCat`, Interface: **XIB** 또는 SwiftUI 대신 **AppKit**, Language: Objective-C (→ `.mm` 로 변경) 또는 Swift
  - 저장 위치: 이 `macos/` 폴더 (Create Git repository 체크 해제)
- `../core/*.h, *.cpp` → **Add Files** 시 "Copy items if needed" **해제** (참조만)
- `../assets/` → 폴더 참조(파란 폴더)로 추가 → Copy Bundle Resources
- Build Settings
  - C++ Language Dialect: C++20
  - Header Search Paths: `$(SRCROOT)/../core`
  - Architectures: Standard (arm64 + x86_64 유니버설)
  - Deployment Target: macOS 13 이상 권장
- Info.plist: `LSUIElement = YES` (Dock 아이콘 숨김, 상주형)
- 권한: 전역 키 입력 감지(CGEventTap)에 **입력 모니터링** 권한 필요

## 규칙
- `core/` 를 이 폴더로 복사하지 않는다
- 플랫폼 코드는 이 폴더 안에만 둔다
