#pragma once

#include "ComboTracker.h"

namespace bongo
{
    enum class Pose
    {
        idle,
        left,
        right,
    };

    enum class SoundId
    {
        none,
        combo,
        combo_high,
    };

    struct SlapResult
    {
        bool accepted{false};
        Pose pose{Pose::idle};
        int slaps{0};
        ComboState combo{};
        SoundId sound{SoundId::none};
    };

    class BongoEngine
    {
    public:
        struct Config
        {
            int max_slaps{0};
            int slaps{0};
        };

        explicit BongoEngine(Config config = {}) noexcept;

        SlapResult OnInput(TimePoint now) noexcept;
        bool Tick(TimePoint now) noexcept;
        void SetPaused(bool paused) noexcept;
        void Reset() noexcept;
        [[nodiscard]] bool IsPaused() const noexcept;
        [[nodiscard]] Pose CurrentPose() const noexcept;
        [[nodiscard]] int Slaps() const noexcept;

    private:
        Config config_{};
        ComboTracker combo_{};
        Pose pose_{Pose::idle};
        TimePoint pose_deadline_{};
        bool has_active_pose_{false};
        bool paused_{false};
    };
}
