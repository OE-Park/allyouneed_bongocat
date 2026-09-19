---
description: 메모리/CPU 점검 및 최적화
---
Release ARM64 빌드 기준으로 리소스 사용을 점검해줘.

1. 측정 방법 안내: 작업 관리자(메모리 = Private Working Set 열 추가), Sysinternals VMMap, VS 진단 도구(메모리 사용량 스냅샷)
2. 코드 리뷰 관점: 불필요한 상시 타이머, 매 프레임 할당, 해제 안 된 GDI/COM 객체, 과도한 D2D 리소스, DLL 로드(`dumpbin /dependents`)
3. 개선안을 영향 큰 순으로 표로 제시하고, 내가 승인한 항목만 수정
4. 목표: 유휴 Private WS ≤ 10 MB, 유휴 CPU 0%, GDI 객체 수 일정
