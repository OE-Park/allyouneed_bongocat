---
description: 효과음 재생
---
WinAudio 를 구현해줘. (SPEC 7장)

- WAV 는 시작 시 메모리에 로드(내장 리소스 + 사용자 폴더 덮어쓰기)
- 1차: `PlaySoundW(SND_MEMORY|SND_ASYNC|SND_NODEFAULT)`
- `sound_volume` 반영이 PlaySound 로 불가능하면 XAudio2(마스터링 보이스 1개, 소스 보이스 소량 풀)로 교체 제안 후 진행
- `sound_enabled=false` 면 오디오 장치를 아예 열지 않기(메모리 절약)

완료 조건: 슬랩/콤보 사운드 재생, 사운드 끈 상태에서 메모리 증가 없음.
