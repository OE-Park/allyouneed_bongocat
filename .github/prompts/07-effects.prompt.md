---
description: 콤보 텍스트·플로팅 +1·총합 표시 (Direct2D/DirectWrite)
---
텍스트 효과를 추가해줘. (SPEC 2·3장)

- `ID2D1DCRenderTarget` 로 캐시된 고양이 비트맵 위에 합성 → `UpdateLayeredWindow`
- 창 크기는 효과가 들어갈 여백을 포함해 고정 (이펙트마다 창 리사이즈 금지)
- 콤보 텍스트: 색 단계·폰트 크기·팝(1.2배 150ms)·페이드(300ms), 그림자
- 플로팅 "+1": 400ms, 40px 상승, 가로 ±15px 랜덤, 동시에 최대 N개(풀링, 예: 16개)
- `always_show_points` 총합 표시
- 애니메이션 타이머는 활성 효과가 있을 때만 동작 → 모두 끝나면 `KillTimer`
- DirectWrite `IDWriteTextFormat` 은 크기별로 캐시

완료 조건: 빠르게 타이핑해도 끊김 없음, 입력 멈추고 1초 뒤 CPU 0%.
