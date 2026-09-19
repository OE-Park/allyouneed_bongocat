#pragma once
// Platform-independent core. Do NOT include OS headers here (shared with macOS).
#include <string_view>

namespace bongo
{
    std::string_view Version() noexcept;
}
