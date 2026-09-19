---
description: 업적 14종 + 알림
---
업적 시스템을 구현해줘. (SPEC 8장)

- core `AchievementManager`: 14개 정의, `achievements.json` 로드/저장(JSON), 시간대 업적은 IClock 의 로컬 시각 사용
- `SlapResult.unlocked` 로 셸에 전달
- win: 고양이 위 말풍선(D2D 둥근 사각형 + 텍스트) 2.5초 표시 후 페이드
- 트레이 메뉴에 "업적 (n/14)" 하위 메뉴(해금 여부 체크 표시, 숨김 업적은 ??? 표시)
- core 테스트: 각 임계값, 중복 해금 방지, 시간대 경계

완료 조건: 슬랩 1회 시 첫 업적 알림 표시.
