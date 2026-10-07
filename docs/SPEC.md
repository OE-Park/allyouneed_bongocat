# BongoCat Native — 기능 명세

> 원본(luinbytes/bongocat, Python/PyQt)의 동작을 기준으로 정리. 값은 원본 `bongo_cat/animations/constants.py` 기준.

## 1. 코어 동작
- 입력(키 down / 마우스 버튼 down / 컨트롤러 버튼) 1회 = **슬랩 1회**
- 포즈: `idle` → 입력 시 `left`/`right` **번갈아** 표시 → **100 ms** 후 `idle` 복귀 (`SLAP_RESET_DELAY_MS`)
- 키 반복(누르고 있기)은 추가 슬랩으로 세지 않음
- 일시정지 상태면 무시
- `max_slaps` > 0 이면 그 값에서 카운터 상한

## 2. 콤보
- 직전 슬랩과 간격 **< 800 ms** → 콤보 +1, 아니면 1로 초기화 (`COMBO_TIMEOUT_MS`)
- 콤보 텍스트 색: < 30 노랑(255,255,100) / ≥ 30 주황(255,150,50) / ≥ 60 빨강(255,50,50)
- 폰트 크기: 14 ~ 20 (콤보/3 비례)
- 팝 효과: 스케일 1.2, 150 ms / 종료 시 페이드 300 ms
- 60+ 콤보 "오버로드" 흔들림 효과: 33 ms 간격, 스케일 0.9~1.2, X ±8 / Y ±5 (선택 구현)
- 콤보 10/25/50/100 도달 시 사운드 (50 이상은 `combo_high`)

## 3. 표시 요소
- 고양이 이미지: 기본 200×126 PNG (투명), 스킨의 `rotation_degrees`(기본 -13°) 적용
- 유휴 "숨쉬기" 스트레치: 상단 75%만 0.98~1.08 배 (선택, 저전력 모드에선 끔)
- `floating_points`: 슬랩마다 "+1" 텍스트가 400 ms 동안 40 px 상승하며 페이드, 가로 ±15 px 랜덤
- `always_show_points`: 우상단에 총 슬랩 수 표시
- 하단 footer(⚙ 설정 / 📄 ini 열기): `hidden_footer`=true 면 마우스 호버 시에만 표시, 투명도 `footer_alpha`
- `invert_cat`: 좌우 반전

## 4. 창 / 상주
- 프레임 없음, 항상 위, 작업표시줄에 안 보임, 드래그로 이동, 종료 시 위치 저장/복원
- 트레이 아이콘 메뉴: 보이기/숨기기, 일시정지, 카운트 리셋, 설정, 종료
- 우클릭 컨텍스트 메뉴: 트레이 메뉴와 동일

## 5. 설정 (INI)
- 위치: `%APPDATA%\BongoCatNative\bongo.ini` (macOS: `~/Library/Application Support/BongoCatNative/bongo.ini`)
- 실행 중 파일 직접 수정은 다음 실행 시 반영 (설정 창 변경은 즉시 반영)

| 키 | 의미 | 기본값 |
|---|---|---|
| `hidden_footer` | 호버 시에만 footer 표시 | `true` |
| `footer_alpha` | footer 불투명도 0–100 | `50` |
| `always_show_points` | 총 슬랩 수 상시 표시 | `false` |
| `floating_points` | "+1" 애니메이션 | `true` |
| `invert_cat` | 좌우 반전 | `false` |
| `startup_with_windows` | 로그인 시 자동 실행 | `false` |
| `max_slaps` | 카운터 상한 (0 = 무제한) | `0` |
| `current_skin` | 스킨 폴더 이름 | `default` |
| `sound_enabled` | 효과음 | `true` |
| `sound_volume` | 볼륨 0–100 | `50` |
| `slaps` | 누적 슬랩 수 | `0` |
| `pos_x`, `pos_y` | 창 위치 | 화면 우하단 |

## 6. 스킨
```
skins/<id>/skin.json, cat-rest.png, cat-left.png, cat-right.png
```
```json
{ "name": "Classic", "author": "…", "version": "1.0.0",
  "images": { "idle": "cat-rest.png", "left": "cat-left.png", "right": "cat-right.png" },
  "rotation_degrees": -13 }
```
- 세 포즈 모두 필수, 누락 시 기본 스킨으로 대체
- 기본 스킨은 exe 리소스에 임베드

## 7. 사운드
- `slap.wav` / `slap_alt.wav`(랜덤 교대), `combo.wav`, `combo_high.wav`, `achievement.wav`
- 500 ms 이하 WAV 권장

## 8. 업적 (14개, `achievements.json` 에 저장)
- 슬랩 수: 1, 100, 500, 1,000, 5,000, 10,000
- 콤보: 10, 25, 50, 100, 200
- 숨김: 0–3시 사이 슬랩, 5–7시 사이 슬랩, 60+ 콤보 유지
- 해금 시 토스트형 알림(고양이 위 작은 말풍선) + `achievement.wav`

## 9. 컨트롤러 (선택 · 마지막 단계)
- Windows: XInput (연결돼 있을 때만 폴링, 미연결 시 5초 간격 재확인)
- 버튼 down 엣지만 슬랩으로 카운트

## 10. 비기능 요구
- Release, 기본 스킨, 유휴 상태: Private Working Set ≤ 10 MB, CPU ≈ 0%
- 관리자 권한 없이 동작
- x64 / ARM64 네이티브 exe 각각 제공, 단일 exe(설치 불필요)
- 참고: 원본은 관리자 권한에서 실행 중인 창 위에서 입력을 못 받을 수 있음(UIPI) — 동일한 한계 허용

## 라이선스 메모
- 원본 README는 MIT라고 명시하지만 저장소에 LICENSE 파일이 없음 → 원본 이미지/사운드 재사용 전 확인. 개발 중엔 자체 placeholder 에셋 사용 권장.
