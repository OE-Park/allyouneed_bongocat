#pragma once

#include "Constants.h"

namespace bongo
{
    class IClock
    {
    public:
        virtual ~IClock() = default;
        virtual TimePoint Now() const noexcept = 0;
    };
}
