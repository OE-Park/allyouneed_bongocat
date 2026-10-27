---
description: 사용자 스킨 로드
---
스킨 시스템을 구현해줘. (SPEC 6장)

- core `SkinManifest`: nlohmann/json 으로 skin.json 파싱, 필수 필드 검증
- win: `%APPDATA%\BongoCatNative\skins\*` 스캔 + 내장 default
- `current_skin` 로드 실패 시 default 로 대체하고 로그
- 스킨 변경 시 이전 비트맵 해제 후 새로 디코딩(메모리 누수 없어야 함)
- 이미지 크기가 달라도 동작(창 크기 재계산)

완료 조건: 원본 저장소의 neon/retro 형식 폴더를 넣고 ini 에서 바꾸면 적용됨.
