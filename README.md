# allyouneed_bongocat

[bongo cat](https://github.com/luinbytes/bongocat)을 적은 메모리로 상주하는 네이티브 앱으로 다시 만드는 프로젝트입니다.

## 폴더
| 폴더 | 용도 | 도구 |
|---|---|---|
| `core/` | 공통 C++20 로직 (OS 무관) | 공용 |
| `windows/` | Win32 앱 (x64 / ARM64) | Visual Studio Community |
| `macos/` | AppKit 앱 (arm64 / x86_64) | Xcode (예정) |
| `assets/` | 스킨·사운드 | 공용 |
| `docs/` | 명세·구조·Copilot 가이드 | — |
| `.github/` | Copilot 지침·프롬프트 | — |

## 빌드 (Windows)

- 요구 사항: Windows 10 이상, Visual Studio 2026의 **C++를 사용한 데스크톱 개발** 워크로드, Windows 10/11 SDK, **C++용 vcpkg 패키지 관리자** 구성 요소
- Visual Studio의 vcpkg 구성 요소를 사용하지 않으면 `VcpkgRoot` 또는 `VcpkgBuildsystemsPath`를 지정
- `allyouneed_bongocat.slnx` 를 Visual Studio로 열기
- 또는 Developer PowerShell:

```powershell
msbuild allyouneed_bongocat.slnx -m -p:Configuration=Release -p:Platform=ARM64
msbuild allyouneed_bongocat.slnx -m -p:Configuration=Release -p:Platform=x64
```

- 결과: `bin\<Platform>\Release\BongoCat.exe`

PR에서는 x64·ARM64의 Debug·Release 빌드를 실행합니다. Microsoft C++ Code Analysis와 C++ Core Check는 x64 Release 빌드에서 실행합니다.

## 개발

- PR·영문 커밋 규칙: [CONTRIBUTING.md](CONTRIBUTING.md)
- `master` 변경은 PR과 필수 CI·Dependency review·CodeQL 검사를 거칩니다.
- 리뷰 대화를 해결한 뒤 squash merge합니다.
- Copilot 코드 리뷰는 PR 생성과 후속 push마다 자동 요청됩니다.
- Dependabot은 GitHub Actions 버전을 매주 확인합니다.
- Secret scanning과 push protection을 사용합니다.

## macOS

현재 앱은 Win32 API를 사용하므로 macOS에서 빌드할 수 없습니다. 플랫폼 독립 로직은 `core/`에 분리하고, macOS 네이티브 타깃이 추가되면 macOS CI도 함께 추가합니다.

## 문서

- [기능 명세](docs/SPEC.md)
- [아키텍처](docs/ARCHITECTURE.md)
- [Copilot 작업 가이드](docs/COPILOT_GUIDE.md)
