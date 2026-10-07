---
description: 투명 레이어드 창 + 고양이 이미지 표시
---
windows/BongoCat 에 LayeredRenderer 와 App 을 구현해 고양이를 화면에 띄워줘.

- WIC 로 PNG → premultiplied BGRA 32bpp DIB, 포즈 3장 시작 시 1회 디코딩·캐시
- 1단계 이미지 소스: 루트의 `assets/skins/default/*.png` 를 `windows/BongoCat/BongoCat.rc` 에 RCDATA 로 임베드 (상대경로 `..\..\assets\...`)
- `WS_POPUP` + `WS_EX_LAYERED|TOPMOST|TOOLWINDOW|NOACTIVATE`, `UpdateLayeredWindow(ULW_ALPHA)`
- `rotation_degrees` 회전과 `invert_cat` 반전은 D2D 변환으로 한 번 그려 캐시
- `WM_NCHITTEST` → 불투명 픽셀이면 `HTCAPTION`(드래그 이동), 투명 픽셀이면 `HTTRANSPARENT`
- DPI 스케일 반영, `WM_DPICHANGED` 처리
- 임시 테스트용: 스페이스 키가 아니라 **창 클릭** 시 engine.OnInput() 호출해서 포즈 교대 확인

완료 조건: 바탕화면 위에 투명 배경 고양이가 보이고, 드래그 이동·클릭 슬랩 동작. 작업 관리자에서 메모리 수치 알려줘(측정 방법 안내).
