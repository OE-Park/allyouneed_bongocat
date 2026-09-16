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
- `allyouneed_bongocat.slnx` 를 Visual Studio로 열기
- 또는 Developer PowerShell:
```powershell
msbuild allyouneed_bongocat.slnx -m -p:Configuration=Release -p:Platform=ARM64
msbuild allyouneed_bongocat.slnx -m -p:Configuration=Release -p:Platform=x64
```
- 결과: `bin\<Platform>\Release\BongoCat.exe`

## 문서
- [기능 명세](docs/SPEC.md)
- [아키텍처](docs/ARCHITECTURE.md)
- [Copilot 작업 가이드](docs/COPILOT_GUIDE.md)
