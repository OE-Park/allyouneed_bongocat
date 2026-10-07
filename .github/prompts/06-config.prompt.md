---
description: INI 설정 로드/저장 (core) + 경로 (win)
---
설정 시스템을 구현해줘. (SPEC 5장)

- core `IniFile`: 자체 파서, 알 수 없는 키/주석 보존, 원자적 저장(임시 파일 → 교체는 셸이 담당)
- core `Config`: SPEC 표의 모든 키, 기본값, 범위 보정(0–100 등)
- win `Paths`: `SHGetKnownFolderPath(FOLDERID_RoamingAppData)` → `BongoCatNative\`, 폴더 없으면 생성, `ReplaceFileW` 로 원자적 저장
- 슬랩 수 저장은 2초 디바운스 타이머 + 종료 시 즉시 저장
- 트레이 메뉴에 "설정 파일 열기"(`ShellExecuteW` "open") 추가
- core 단위 테스트: 파싱/보존/기본값/범위

완료 조건: 앱 종료→재실행 시 슬랩 수·위치 유지.
