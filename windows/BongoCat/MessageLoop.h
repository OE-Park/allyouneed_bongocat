#pragma once

namespace bongo::win32
{
    enum class MessageResult
    {
        error,
        quit,
        dispatch,
    };

    constexpr MessageResult ClassifyGetMessageResult(int result) noexcept
    {
        if (result < 0)
        {
            return MessageResult::error;
        }
        if (result == 0)
        {
            return MessageResult::quit;
        }
        return MessageResult::dispatch;
    }

    static_assert(ClassifyGetMessageResult(-1) == MessageResult::error);
    static_assert(ClassifyGetMessageResult(0) == MessageResult::quit);
    static_assert(ClassifyGetMessageResult(1) == MessageResult::dispatch);
}
