#pragma once

#include "Constants.h"

namespace bongo
{
    enum class ComboColor
    {
        yellow,
        orange,
        red,
    };

    struct ComboState
    {
        int count{};
        ComboColor color{ComboColor::yellow};
        int font_size{MinimumComboFontSize};
    };

    class ComboTracker
    {
    public:
        ComboState OnSlap(TimePoint now) noexcept;
        void Reset() noexcept;
        [[nodiscard]] const ComboState& State() const noexcept;

    private:
        ComboState state_{};
        TimePoint last_slap_{};
        bool has_slap_{false};
    };
}
