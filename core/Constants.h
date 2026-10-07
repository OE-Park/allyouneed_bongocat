#pragma once

#include <chrono>

namespace bongo
{
    using Duration = std::chrono::milliseconds;
    using TimePoint = std::chrono::steady_clock::time_point;

    inline constexpr Duration SlapResetDelay{100};
    inline constexpr Duration ComboTimeout{800};
    inline constexpr Duration ComboPopDuration{150};
    inline constexpr Duration ComboFadeDuration{300};
    inline constexpr int ComboOrangeThreshold = 30;
    inline constexpr int ComboRedThreshold = 60;
    inline constexpr int MinimumComboFontSize = 14;
    inline constexpr int MaximumComboFontSize = 20;
    inline constexpr int ComboFontGrowth = 3;
}
