---
description: 플랫폼 독립 코어(core) 규칙 — macOS 재사용 전제
applyTo: "core/**"
---

# core 규칙
- OS 헤더(`windows.h`, `Cocoa`, `wil` 등) include 금지. C++20 표준 라이브러리만.
- 시간은 직접 `std::chrono::steady_clock` 호출 대신 `IClock` 인터페이스로 주입 → 테스트에서 가짜 시계 사용.
- 파일 경로는 `std::filesystem::path`. 설정 파일 위치 결정은 플랫폼 셸 책임(core는 경로를 받기만).
- 예외는 로드/파싱 실패에만 사용하고, 입력 처리(`OnInput`) 경로는 `noexcept`.
- 상태 변화는 반환값/이벤트 구조체로 셸에 알린다 (예: `SlapResult { Pose pose; int combo; std::vector<AchievementId> unlocked; SoundId sound; }`). core가 렌더링·사운드를 직접 호출하지 않는다.
- 모든 public 함수는 `tests/` 에 단위 테스트를 추가한다.
- INI 파서는 자체 구현(주석 `;`/`#`, `[section]`, `key=value`, bool은 `true/false/1/0`). 알 수 없는 키는 보존해서 다시 저장.
