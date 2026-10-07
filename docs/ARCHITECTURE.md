# BongoCat Native — 아키텍처

## 레이어
```
┌──────────────── windows/BongoCat (Win32 셸) ─────────────────┐   ┌──── macos (나중) ────┐
│ WinMain · App(메시지 루프, 타이머)                   │   │ AppKit NSPanel          │
│ RawInputSource   → IInputSource                     │   │ CGEventTap              │
│ LayeredRenderer  → IRenderer (WIC+D2D+ULW)          │   │ CALayer / CoreGraphics  │
│ WinAudio         → IAudio (PlaySound→XAudio2)       │   │ AVAudioPlayer           │
│ Tray · Menu · SettingsDialog · Autostart            │   │ NSStatusItem            │
└───────────────────────┬─────────────────────────────┘   └───────────┬────────────┘
                        │  인터페이스 호출 / 이벤트 구조체              │
┌───────────────────────┴──────────── core (C++20, OS 무관) ───────┴────────────┐
│ BongoEngine: OnInput() → SlapResult, Tick(now) → 애니메이션 상태                   │
│ ComboTracker · AchievementManager · Config(INI) · SkinManifest · Constants         │
└────────────────────────────────────────────────────────────────────────────────────┘
```

## 핵심 흐름
1. `WM_INPUT` → `RawInputSource`가 "새 down 엣지"만 걸러 `engine.OnInput(now)` 호출
2. `SlapResult` 수신 → 포즈 이미지 교체, 100 ms 복귀 타이머, 사운드, 플로팅 텍스트/콤보 애니메이션 시작
3. 애니메이션 타이머(≈60 fps)는 **활성 애니메이션이 있을 때만** 동작 → 모두 끝나면 정지
4. 슬랩 수 변경 → 2 s 디바운스 저장 타이머

## 파일 구성(목표)
```
allyouneed_bongocat/                (git 루트)
├─ allyouneed_bongocat.slnx         VS 솔루션 (루트 고정 — Copilot 에이전트 접근 범위)
├─ vcpkg.json                       (01단계에서 추가: wil, nlohmann-json)
├─ .github/  docs/
├─ assets/   skins/default/*  sounds/default/*
├─ core/                            플랫폼 독립 C++ (Windows·macOS 공용)
│    BongoCore.{h,cpp} BongoEngine.{h,cpp} ComboTracker.{h,cpp} Achievements.{h,cpp}
│    Config.{h,cpp} IniFile.{h,cpp} Skin.{h,cpp} Constants.h Interfaces.h
├─ windows/                         Visual Studio 전용
│  ├─ BongoCommon.props
│  ├─ BongoCore/BongoCore.vcxproj   (core/*.cpp 참조)
│  ├─ BongoCat/  BongoCat.vcxproj main.cpp App.{h,cpp} LayeredRenderer.{h,cpp}
│  │             RawInputSource.{h,cpp} Tray.{h,cpp} SettingsDialog.{h,cpp}
│  │             WinAudio.{h,cpp} Paths.{h,cpp} resource.h BongoCat.rc app.manifest
│  └─ BongoCoreTests/               (02단계에서 추가)
├─ macos/                           Xcode 전용 (나중)
├─ bin/  obj/                       빌드 산출물 (git 제외)
```

## 프로젝트 구성
| 프로젝트 | 종류 | 비고 |
|---|---|---|
| `BongoCore` | 정적 라이브러리(.lib) | windows/BongoCore, 소스는 core/ |
| `BongoCat` | Windows 데스크톱 앱(.exe) | windows/BongoCat, BongoCore 참조 |
| `BongoCoreTests` | Native Unit Test | windows/BongoCoreTests, BongoCore 참조 |

- 공통: C++20, `/MT`, Unicode, `/W4 /permissive-`, Release `/O1 /GL` + `/LTCG /OPT:REF /OPT:ICF`
- 플랫폼: x64, ARM64 (Win32 삭제)

## macOS 이식 계획 (요약)
- `macos/BongoCat.xcodeproj` 에서 `../core/*.cpp` 를 **참조로 추가**(Copy items 해제) → 그대로 재사용
- `assets/` 도 참조로 추가해 앱 번들 Resources 로 복사
- 셸은 Objective-C++(`.mm`) 또는 Swift + C 브리지
- 전역 입력: `CGEventTap`(listen-only) → **입력 모니터링 권한** 필요
- 창: borderless `NSPanel`, `level = .floating`, `collectionBehavior = canJoinAllSpaces`
- 트레이: `NSStatusItem`, 자동 실행: `SMAppService`
- 유니버설 바이너리(arm64 + x86_64)
