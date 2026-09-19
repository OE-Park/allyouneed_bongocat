---
description: Raw Input 전역 키보드/마우스 감지
---
RawInputSource 를 구현해서 전역 입력으로 슬랩이 되게 해줘.

- `RegisterRawInputDevices`: 키보드(0x01/0x06), 마우스(0x01/0x02), `RIDEV_INPUTSINK`, 대상 hwnd 지정
- 키보드: VKey 별 down 상태를 `std::bitset<256>`로 추적 → 최초 down 만 슬랩, `RI_KEY_BREAK` 에서 해제
- 마우스: L/R/M/X1/X2 버튼 DOWN 플래그만. 이동·휠 무시
- 스택 버퍼로 `GetRawInputData`, 할당/로그 금지
- 포커스를 잃었을 때 눌린 키 상태를 초기화할 방법도 고려(`WM_INPUT_DEVICE_CHANGE`, 일정 시간 무입력 시 리셋 등)
- 03단계의 "클릭 시 슬랩" 임시 코드는 제거

완료 조건: 다른 앱(메모장 등)에 타이핑해도 고양이가 반응, 키를 꾹 눌러도 1회만 카운트. 유휴 CPU 0% 확인.
