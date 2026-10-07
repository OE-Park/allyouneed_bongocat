---
description: Win32 셸(windows/BongoCat) 규칙 — 저메모리 레이어드 창
applyTo: "windows/**"
---

# windows/BongoCat 규칙
- 렌더 파이프라인: WIC 디코드 → premultiplied BGRA `HBITMAP`(DIB section) → 필요 시 `ID2D1DCRenderTarget`으로 텍스트/효과 합성 → `UpdateLayeredWindow(ULW_ALPHA)`.
- Direct2D/DirectWrite 팩토리는 앱 수명 동안 1개만 생성. 렌더 타깃은 크기 변경 시에만 재생성.
- `WM_PAINT` 사용하지 않음(레이어드 창). 화면 갱신은 `Renderer::Present()` 한 곳에서만.
- 타이머 ID는 `enum class TimerId` 로 관리. 사용 후 반드시 `KillTimer`.
- Raw Input: `WM_INPUT` 에서 `GetRawInputData`를 스택 버퍼로 처리. 키보드는 `RI_KEY_BREAK` 로 up 판별, 마우스는 `RI_MOUSE_*_BUTTON_DOWN` 만 카운트(이동/휠 무시).
- 트레이: `Shell_NotifyIconW` + `NOTIFYICON_VERSION_4`, `TaskbarCreated` 메시지 재등록 처리(탐색기 재시작 대응).
- 설정 창: `.rc` 다이얼로그 리소스 + `DialogBoxParamW`(모달). 닫으면 리소스 해제.
- 시작 프로그램 등록: `HKCU\Software\Microsoft\Windows\CurrentVersion\Run` 사용(관리자 권한 불필요).
- 사운드: 1단계는 `PlaySoundW(SND_MEMORY | SND_ASYNC)`, 겹침 재생이 필요해지면 XAudio2로 교체.
- 모든 Win32 실패는 `LOG_LAST_ERROR_IF` 등으로 기록하되, 입력 경로에서는 로깅하지 않는다.
