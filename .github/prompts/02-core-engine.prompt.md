---
description: 플랫폼 독립 코어 엔진 + 단위 테스트
---
core 에 BongoEngine 을 구현해줘. (docs/SPEC.md 1·2장)

- `Interfaces.h`: `IClock`, `IRenderer`, `IInputSource`, `IAudio` 인터페이스 (최소 메서드만)
- `Constants.h`: SPEC 에 나온 수치 전부 constexpr 로
- `ComboTracker`: 800 ms 규칙, 색상 단계(Yellow/Orange/Red), 폰트 크기 계산
- `BongoEngine`:
  - `SlapResult OnInput(TimePoint now) noexcept` — 포즈 교대, 카운트 증가(max_slaps 반영), 콤보, 재생할 사운드 ID
  - `bool Tick(TimePoint now)` — 100 ms 후 idle 복귀 등, 아직 애니메이션이 남아 있으면 true
  - 일시정지 / 리셋
- `windows/BongoCoreTests/BongoCoreTests.vcxproj` (Native Unit Test, x64/ARM64만, slnx 에 추가): 가짜 시계로 교대·콤보 초기화·상한·일시정지·사운드 마일스톤(10/25/50/100) 테스트

완료 조건: 테스트 전부 통과(ARM64). core 에 OS 헤더 include 없음.
