---
description: 설정 다이얼로그 + 자동 실행
---
설정 창을 만들어줘.

- .rc 다이얼로그 리소스: SPEC 5장 항목 전부 (체크박스, 트랙바, 스킨 콤보박스, 숫자 입력)
- DPI 대응 폰트(`SystemParametersInfoW(SPI_GETNONCLIENTMETRICS)`), 다크 모드는 선택
- 확인 시 즉시 적용 + 저장, 취소 시 원복
- `startup_with_windows`: `HKCU\...\Run` 에 exe 경로(따옴표 포함) 등록/삭제
- footer(⚙/📄) 호버 표시와 `hidden_footer`, `footer_alpha` 구현

완료 조건: 모든 옵션이 재시작 없이 반영, 재부팅 후 자동 실행 확인.
