#include "ComboTracker.h"

#include <algorithm>

namespace bongo
{
    ComboState ComboTracker::OnSlap(const TimePoint now) noexcept
    {
        if (has_slap_ && now - last_slap_ < ComboTimeout)
        {
            ++state_.count;
        }
        else
        {
            state_.count = 1;
        }

        state_.color = state_.count >= ComboRedThreshold
            ? ComboColor::red
            : state_.count >= ComboOrangeThreshold ? ComboColor::orange : ComboColor::yellow;
        state_.font_size = std::min(
            MaximumComboFontSize,
            MinimumComboFontSize + state_.count / ComboFontGrowth);
        last_slap_ = now;
        has_slap_ = true;
        return state_;
    }

    void ComboTracker::Reset() noexcept
    {
        state_ = {};
        last_slap_ = {};
        has_slap_ = false;
    }

    const ComboState& ComboTracker::State() const noexcept
    {
        return state_;
    }
}
