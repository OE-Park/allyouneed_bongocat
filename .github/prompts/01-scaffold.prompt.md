---
description: 기존 솔루션 뼈대 점검·보완 (vcpkg 추가)
---
솔루션 뼈대는 이미 있다 (docs/ARCHITECTURE.md "파일 구성" 참고). 새로 만들지 말고 점검·보완만 해줘.

1. `allyouneed_bongocat.slnx`, `windows/BongoCommon.props`, `windows/BongoCore/BongoCore.vcxproj`, `windows/BongoCat/BongoCat.vcxproj` 를 읽고 copilot-instructions.md 의 빌드 규칙과 다른 점을 보고
2. 루트에 `vcpkg.json`(manifest 모드) 추가: `wil`, `nlohmann-json`
   - 터미널에서 `vcpkg x-update-baseline --add-initial-baseline` 로 baseline 추가
   - `BongoCommon.props` 에 `<VcpkgEnableManifest>true</VcpkgEnableManifest>` 추가
3. `main.cpp` 에서 `<wil/resource.h>` include 가 되는지 확인
4. x64 / ARM64 Release·Debug 네 가지 모두 빌드

완료 조건: 네 가지 구성 모두 빌드 성공, 실행 시 빈 창 표시.
