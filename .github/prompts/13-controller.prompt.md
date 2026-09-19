---
description: (선택) XInput 컨트롤러 입력
---
컨트롤러 입력을 추가해줘. (SPEC 9장)

- `XInputGetState`(xinput1_4) 사용, 연결된 패드가 있을 때만 16ms 폴링, 없으면 5초 간격 재확인
- `wButtons` + 트리거 임계값의 down 엣지만 슬랩
- 설정에 `controller_enabled`(기본 false) 추가
- ARM64 에서도 동작 확인 방법 안내

완료 조건: 패드 버튼으로 슬랩, 패드 미연결 시 CPU 0%.
