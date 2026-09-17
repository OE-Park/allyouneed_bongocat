#include "BongoEngine.h"

namespace bongo
{
    BongoEngine::BongoEngine(const Config config) noexcept
        : config_(config)
    {
        if (config_.max_slaps > 0 && config_.slaps > config_.max_slaps)
        {
            config_.slaps = config_.max_slaps;
        }
    }

    SlapResult BongoEngine::OnInput(const TimePoint now) noexcept
    {
        SlapResult result{};
        result.pose = pose_;
        result.slaps = config_.slaps;
        result.combo = combo_.State();

        if (paused_)
        {
            return result;
        }

        if (config_.max_slaps == 0 || config_.slaps < config_.max_slaps)
        {
            ++config_.slaps;
        }

        pose_ = pose_ == Pose::left ? Pose::right : Pose::left;
        pose_deadline_ = now + SlapResetDelay;
        has_active_pose_ = true;
        result.accepted = true;
        result.pose = pose_;
        result.slaps = config_.slaps;
        result.combo = combo_.OnSlap(now);
        if (result.combo.count == 10 || result.combo.count == 25)
        {
            result.sound = SoundId::combo;
        }
        else if (result.combo.count == 50 || result.combo.count == 100)
        {
            result.sound = SoundId::combo_high;
        }
        return result;
    }

    bool BongoEngine::Tick(const TimePoint now) noexcept
    {
        if (!has_active_pose_)
        {
            return false;
        }
        if (now < pose_deadline_)
        {
            return true;
        }

        pose_ = Pose::idle;
        has_active_pose_ = false;
        return false;
    }

    void BongoEngine::SetPaused(const bool paused) noexcept
    {
        paused_ = paused;
        if (paused_)
        {
            has_active_pose_ = false;
            pose_ = Pose::idle;
        }
    }

    void BongoEngine::Reset() noexcept
    {
        config_.slaps = 0;
        combo_.Reset();
        pose_ = Pose::idle;
        pose_deadline_ = {};
        has_active_pose_ = false;
    }

    bool BongoEngine::IsPaused() const noexcept
    {
        return paused_;
    }

    Pose BongoEngine::CurrentPose() const noexcept
    {
        return pose_;
    }

    int BongoEngine::Slaps() const noexcept
    {
        return config_.slaps;
    }
}
