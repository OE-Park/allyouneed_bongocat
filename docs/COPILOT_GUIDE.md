# BongoCat Native — Visual Studio + GitHub Copilot 작업 키트

## 1. 구성
```
.github/
  copilot-instructions.md        ← 모든 요청에 자동 적용되는 공통 지침
  instructions/
    core.instructions.md         ← core/** 에만 적용
    win32.instructions.md        ← windows/** 에만 적용
  prompts/
    00-plan … 13-controller      ← 단계별 프롬프트
    fix-build / review           ← 반복해서 쓰는 프롬프트
docs/
  SPEC.md                        ← 기능 명세 (원본 기준 수치 포함)
  ARCHITECTURE.md                ← 폴더 구조 / 프로젝트 구성 / macOS 이식 계획
  COPILOT_GUIDE.md               ← 이 문서
```

## 2. 사전 준비
- **Visual Studio Installer**
  - 워크로드: "C++를 사용한 데스크톱 개발"
  - 개별 구성 요소: **MSVC ARM64 빌드 도구**, MSVC x64 빌드 도구, Windows 11 SDK, **C++용 vcpkg 패키지 관리자**
- **Copilot 설정** (도구 > 옵션 > GitHub > Copilot > Copilot Chat)
  - ✅ *Enable custom instructions to be loaded from .github/copilot-instructions.md files and added to requests*
  - ✅ *Enable Agent mode in the chat pane* (17.14 이상)
- **폴더 준비**
  - 루트의 `allyouneed_bongocat.slnx` 를 VS에서 열기 — 솔루션은 루트에서 옮기지 않기
  - 솔루션·프로젝트 폴더는 준비 완료
  - 03 창 단계 전에 `assets/skins/default/` 에 200×126 `cat-rest.png`, `cat-left.png`, `cat-right.png`와 `skin.json` 준비
    - 자체 제작 자산이나 재배포가 명시적으로 허용된 자산만 사용하고 출처·라이선스를 기록
    - 원본 저장소는 LICENSE 파일이 없으므로 재배포 권한 확인 전까지 원본 이미지를 사용하지 않기
- **처음 한 번**: 솔루션을 x64·ARM64로 각각 빌드해 빈 창이 뜨는지 확인

## 3. 진행 방법
| 단계 | 모드 | 사용법 | 확인 |
|---|---|---|---|
| 00 계획 | **Plan** (또는 Ask) | 채팅에 `/00-plan` | docs/PLAN.md 검토·수정 |
| 01 뼈대 보완 | Agent | `/01-scaffold` | vcpkg 추가, x64·ARM64 빌드 성공 |
| 02 코어 | Agent | `/02-core-engine` | 테스트 탐색기 전부 통과 |
| 03 창 | Agent | `/03-layered-window` | 투명 고양이 표시·드래그 |
| 04 입력 | Agent | `/04-raw-input` | 다른 앱 타이핑에 반응 |
| 05 트레이 | Agent | `/05-tray-menu` | 트레이 메뉴 동작 |
| 06 설정 | Agent | `/06-config` | 재실행 후 값 유지 |
| 07 효과 | Agent | `/07-effects` | 콤보·+1 표시, 유휴 CPU 0% |
| 08 스킨 | Agent | `/08-skins` | 스킨 교체 |
| 09 사운드 | Agent | `/09-sound` | 효과음 |
| 10 업적 | Agent | `/10-achievements` | 첫 슬랩 알림 |
| 11 설정 창 | Agent | `/11-settings-dialog` | 즉시 반영 |
| 12 점검 | Ask → Agent | `/12-memory-audit` | ≤ 10 MB |
| 13 패드 | Agent | `/13-controller` | (선택) |

- 프롬프트 호출
  - `/` 입력 → 목록에서 선택
  - 또는 `#prompt:` 입력 → 파일 선택
- 단계마다 반복할 루틴
  1. 프롬프트 실행 → 변경 내용 diff 검토 → **Keep / Undo**
  2. 빌드 오류가 나면 `/fix-build`
  3. `/review` 로 규칙 위반 점검
  4. 직접 실행해서 "완료 조건" 확인 → **git commit** (단계마다 커밋 = 되돌리기 쉬움)
- 프롬프트 뒤에 추가 요구를 붙여도 됨
  - 예: `/07-effects 콤보 색은 파스텔 톤으로`

## 4. 효과를 높이는 팁
- **대화는 단계마다 새로 시작** (채팅 창 "새 스레드")
  - 이전 단계의 긴 문맥이 섞이면 품질이 떨어짐
- **모델 선택**
  - 계획·리뷰·설계: 추론 강한 모델
  - 반복 수정·빌드 루프: 빠른 모델
- **파일 첨부**: `#` 로 관련 파일(`#BongoEngine.h` 등)을 직접 붙이면 엉뚱한 파일 수정이 줄어듦
- **터미널 명령 승인**
  - msbuild / vstest 는 허용
  - 삭제·레지스트리 변경 명령은 내용을 확인하고 승인
- **Copilot이 규칙을 어기면**
  - `copilot-instructions.md 의 "성능·메모리 규칙"을 다시 읽고 고쳐줘` 로 되돌리기
- **지침 개선**
  - 반복해서 지적하는 내용은 `copilot-instructions.md` 에 한 줄씩 추가
  - `/generateInstructions` 로 자동 생성할 수도 있지만, 기존 파일을 덮어쓸 수 있으니 비교 후 병합
- **좋았던 요청 저장**: `/savePrompt` → `.github/prompts/` 에 저장됨

## 5. 수동 빌드 명령 (Developer PowerShell for VS)
```powershell
msbuild allyouneed_bongocat.slnx -m -p:Configuration=Release -p:Platform=ARM64
msbuild allyouneed_bongocat.slnx -m -p:Configuration=Release -p:Platform=x64
```
- 산출물 (저장소 루트 기준)
  - `bin\ARM64\Release\BongoCat.exe`
  - `bin\x64\Release\BongoCat.exe`
- x64 exe 도 Snapdragon PC에서 에뮬레이션으로 실행해 확인 가능

## 6. 이후 macOS
- `core` 는 OS 헤더 없이 유지 → 그대로 재사용
- Mac에서 `macos/` 에 Xcode 프로젝트 생성 (macos/README.md 참고)
- 이식 시작 시 docs/ARCHITECTURE.md "macOS 이식 계획"을 첨부
- 같은 방식으로 `mac.instructions.md`(applyTo: `macos/**`) 와 단계별 프롬프트를 추가
- 빌드: Xcode 또는 CMake (Visual Studio로는 macOS 앱을 빌드할 수 없음)

## 참고
- 원본 저장소: https://github.com/luinbytes/bongocat
- [Customize chat responses and set context (Visual Studio)](https://learn.microsoft.com/visualstudio/ide/copilot-chat-context?view=visualstudio)
- [Get started with GitHub Copilot agent mode](https://learn.microsoft.com/visualstudio/ide/copilot-agent-mode?view=visualstudio)
