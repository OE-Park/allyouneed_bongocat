# BongoCat Native — Copilot 공통 지침

## 프로젝트 개요
- 원본: https://github.com/luinbytes/bongocat (Python + PyQt) → **C++ 네이티브 앱**으로 다시 만든다.
- 목표: 키/마우스 입력 시 고양이가 좌우 발을 번갈아 치는 **데스크톱 펫**. 프레임 없는 창, 항상 위에 표시, 드래그 이동, 트레이 상주.
- 최우선 가치: **작은 상주 메모리 · 유휴 시 CPU 0%에 가깝게 · 빠른 시작**.
- 지원 대상: Windows 10/11 **x64 + ARM64** (주 개발기: Snapdragon X Elite, ARM64). 나중에 **macOS** 이식 예정.
- 기능 명세: `docs/SPEC.md` / 구조: `docs/ARCHITECTURE.md` — 작업 전에 반드시 읽고, 명세와 다르게 구현하면 이유를 설명한다.

## 기술 스택 (변경 금지)
- 언어: **C++20**, MSVC (Visual Studio Community), `/permissive-`, `/W4`, 경고 0 유지.
- UI: **순수 Win32 API** + **WIC**(PNG 디코딩) + **Direct2D/DirectWrite**(텍스트·효과) + `UpdateLayeredWindow`(픽셀 단위 투명).
- 금지: Electron, WebView2, Qt, WinUI, WPF, WinForms, .NET, C++/CLI, MFC, ATL(단, `CComPtr` 대신 `wil`/`Microsoft::WRL::ComPtr` 사용 가능).
- 외부 의존성은 최소화. 허용: WIL(`wil`), nlohmann/json(skin.json 파싱) — vcpkg manifest(`vcpkg.json`)로만 추가.
- 런타임: 정적 CRT(`/MT`, Debug는 `/MTd`) → VC++ 재배포 패키지 불필요.
- 문자열: 내부는 UTF-8 `std::string`, Win32 호출 경계에서만 `std::wstring`으로 변환. 항상 `W` API 사용.

## 폴더 구조 규칙
- `core/` : **플랫폼 독립 코드만**. `<windows.h>` 등 OS 헤더 include 금지. 표준 C++만 사용. (macOS에서 그대로 재사용)
  - 상태 머신(idle/left/right), 콤보, 업적, 설정(ini) 파서, 스킨 메타데이터, 시간 추상화.
- `windows/` : Visual Studio 전용. 솔루션 파일은 **저장소 루트**의 `allyouneed_bongocat.slnx` (옮기지 말 것).
  - `windows/BongoCommon.props` : 모든 vcxproj 공통 설정
  - `windows/BongoCore/` : `core/*.cpp` 를 빌드하는 정적 lib 프로젝트 (소스는 core/ 에 두고 **참조만**, 복사 금지)
  - `windows/BongoCat/` : Win32 셸(exe). core의 인터페이스(`IRenderer`, `IInputSource`, `IAudio` 등)를 구현.
  - `windows/BongoCoreTests/` : core 단위 테스트 (Microsoft Native Unit Test, `CppUnitTestFramework`).
- `macos/` : Xcode 전용 (나중). AppKit 셸. 지금은 README 외 수정하지 않는다.
- `assets/` : 기본 스킨/사운드 (Windows·macOS 공용). Windows는 .rc로 exe에 임베드, 사용자 스킨은 `%APPDATA%\BongoCatNative\skins\` 에서 로드.
- 빌드 산출물: `bin\<Platform>\<Configuration>\`, 중간 파일: `obj\...` (둘 다 git 제외)

## 성능·메모리 규칙 (가장 중요)
- 목표: Release 빌드 **Private Working Set ≤ 10 MB**, 유휴 CPU ≈ 0%.
- **상시 16ms 타이머 금지.** 애니메이션이 필요한 동안에만 타이머를 켜고 끝나면 `KillTimer`. 창이 숨김/일시정지면 모든 타이머 중지.
- 입력 감지는 **Raw Input(`RegisterRawInputDevices` + `RIDEV_INPUTSINK`)** 사용. 저수준 훅(`WH_KEYBOARD_LL`)은 대안으로만.
  - 키 반복(auto-repeat)은 무시: 키 다운 상태를 비트셋으로 추적해 최초 down만 카운트.
  - 입력 처리 경로에서 할당·파일 I/O·로그 출력 금지.
- 이미지는 시작 시 한 번 디코딩해 premultiplied BGRA로 캐시. 매 프레임 재디코딩 금지.
- 슬랩 카운트 저장은 **디바운스**(마지막 입력 후 2초, 종료 시 즉시). 입력마다 파일 쓰기 금지.
- `SetProcessWorkingSetSize` 같은 편법으로 수치만 줄이지 않는다.

## Win32 코딩 규칙
- 매니페스트: Per-Monitor DPI Aware V2, Common Controls v6, `asInvoker`(관리자 권한 요구 금지).
- 창: `WS_POPUP` + `WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE`.
- 드래그: `WM_NCHITTEST` → `HTCAPTION` 방식 우선.
- COM: `wil::com_ptr` 또는 `ComPtr`, HRESULT는 `THROW_IF_FAILED`/`RETURN_IF_FAILED`. 원시 `Release()` 호출 금지.
- 핸들: `wil::unique_hicon`, `wil::unique_hbitmap` 등 RAII 래퍼 사용.
- 전역 변수 최소화 — `App` 클래스 하나가 상태 소유, `GWLP_USERDATA`로 연결.
- `WM_DPICHANGED`, `WM_DISPLAYCHANGE` 처리. 모니터 밖 좌표 복원 시 주 모니터로 보정.

## 빌드 규칙
- 솔루션: `allyouneed_bongocat.slnx`, 플랫폼 **x64 / ARM64** 둘 다 항상 빌드 가능해야 한다. (Win32(x86)는 만들지 않음)
- `#ifdef _M_X64` 같은 아키텍처 분기 코드는 **쓰지 않는다**. 필요하면 먼저 이유를 설명.
- 코드 수정 후에는 Developer PowerShell에서 아래 두 명령이 모두 성공해야 완료로 본다:
  - `msbuild allyouneed_bongocat.slnx -m -p:Configuration=Release -p:Platform=ARM64`
  - `msbuild allyouneed_bongocat.slnx -m -p:Configuration=Release -p:Platform=x64`
- 테스트: `vstest.console.exe` 로 core 테스트 실행 (ARM64 구성).

## 작업 방식
- 한 번에 **한 단계**만 구현한다. 요청 범위를 넘는 파일은 수정하지 않는다.
- 새 파일을 만들면 `.vcxproj` / `.vcxproj.filters` 에도 추가한다.
- 모르는 API 동작은 추측하지 말고 "확인 필요"라고 표시한다.
- 매직 넘버는 `core/Constants.h` 에 모은다 (원본 `animations/constants.py` 값 기준).
- 코드 주석은 영어, **채팅 답변은 한국어·짧은 개조식**.
- 작업 끝에 요약: ① 변경 파일 목록 ② 빌드 결과(x64/ARM64) ③ 수동 확인 방법 ④ 남은 TODO.
