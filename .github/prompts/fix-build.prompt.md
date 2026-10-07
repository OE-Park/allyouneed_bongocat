---
description: 빌드 오류 수정 루프
---
x64 와 ARM64 Release 를 모두 빌드하고, 오류/경고가 없어질 때까지 고쳐줘.

- 명령: `msbuild allyouneed_bongocat.slnx -m -p:Configuration=Release -p:Platform=ARM64` 그리고 `-p:Platform=x64`
- 한 번에 원인 하나씩 수정, 수정 이유를 한 줄로 기록
- 경고를 `#pragma warning(disable)` 로 숨기지 말 것
- 아키텍처 분기(`#ifdef _M_ARM64`)로 우회하지 말 것
