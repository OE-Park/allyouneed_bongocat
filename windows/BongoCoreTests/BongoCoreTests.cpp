#include "CppUnitTest.h"

#include "BongoEngine.h"
#include "ComboTracker.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BongoCoreTests
{
    class FakeClock final : public bongo::IClock
    {
    public:
        bongo::TimePoint Now() const noexcept override
        {
            return now_;
        }

        void Advance(bongo::Duration duration) noexcept
        {
            now_ += duration;
        }

    private:
        bongo::TimePoint now_{};
    };

    TEST_CLASS(ComboTrackerTests)
    {
    public:
        TEST_METHOD(FirstSlapStartsCombo)
        {
            bongo::ComboTracker tracker;
            const auto state = tracker.OnSlap(bongo::TimePoint{});

            Assert::AreEqual(1, state.count);
            Assert::AreEqual(static_cast<int>(bongo::ComboColor::yellow), static_cast<int>(state.color));
            Assert::AreEqual(14, state.font_size);
        }

        TEST_METHOD(ComboResetsAfterTimeout)
        {
            bongo::ComboTracker tracker;
            tracker.OnSlap(bongo::TimePoint{});
            const auto state = tracker.OnSlap(bongo::TimePoint{} + bongo::ComboTimeout);

            Assert::AreEqual(1, state.count);
        }

        TEST_METHOD(ComboUsesColorAndFontThresholds)
        {
            bongo::ComboTracker tracker;
            bongo::ComboState state{};
            for (int i = 0; i < 60; ++i)
            {
                state = tracker.OnSlap(bongo::TimePoint{} + std::chrono::milliseconds(i * 100));
            }

            Assert::AreEqual(60, state.count);
            Assert::AreEqual(static_cast<int>(bongo::ComboColor::red), static_cast<int>(state.color));
            Assert::AreEqual(20, state.font_size);
        }

        TEST_METHOD(ResetClearsComboState)
        {
            bongo::ComboTracker tracker;
            tracker.OnSlap(bongo::TimePoint{});
            tracker.Reset();

            const auto state = tracker.State();
            Assert::AreEqual(0, state.count);
            Assert::AreEqual(static_cast<int>(bongo::ComboColor::yellow), static_cast<int>(state.color));
            Assert::AreEqual(14, state.font_size);
        }
    };

    TEST_CLASS(BongoEngineTests)
    {
    public:
        TEST_METHOD(SlapsAlternatePosesAndReturnToIdle)
        {
            FakeClock clock;
            bongo::BongoEngine engine(clock);
            const auto first = engine.OnInput();
            clock.Advance(std::chrono::milliseconds(10));
            const auto second = engine.OnInput();

            Assert::AreEqual(static_cast<int>(bongo::Pose::left), static_cast<int>(first.pose));
            Assert::AreEqual(static_cast<int>(bongo::Pose::right), static_cast<int>(second.pose));
            clock.Advance(std::chrono::milliseconds(99));
            Assert::IsTrue(engine.Tick());
            clock.Advance(std::chrono::milliseconds(1));
            Assert::IsFalse(engine.Tick());
            Assert::AreEqual(static_cast<int>(bongo::Pose::idle), static_cast<int>(engine.CurrentPose()));
        }

        TEST_METHOD(MaxSlapsCapsCounter)
        {
            FakeClock clock;
            bongo::BongoEngine engine(clock, {2});
            engine.OnInput();
            clock.Advance(std::chrono::milliseconds(10));
            engine.OnInput();
            clock.Advance(std::chrono::milliseconds(10));
            const auto result = engine.OnInput();

            Assert::AreEqual(2, result.slaps);
            Assert::AreEqual(2, engine.Slaps());
        }

        TEST_METHOD(NegativeMaxSlapsIsUnlimited)
        {
            FakeClock clock;
            bongo::BongoEngine engine(clock, {-1});
            const auto result = engine.OnInput();

            Assert::IsTrue(result.accepted);
            Assert::AreEqual(1, result.slaps);
        }

        TEST_METHOD(PausedEngineIgnoresInputAndCanResume)
        {
            FakeClock clock;
            bongo::BongoEngine engine(clock);
            engine.SetPaused(true);
            Assert::IsTrue(engine.IsPaused());
            const auto paused = engine.OnInput();
            Assert::IsFalse(paused.accepted);

            engine.SetPaused(false);
            Assert::IsFalse(engine.IsPaused());
            const auto resumed = engine.OnInput();
            Assert::IsTrue(resumed.accepted);
        }

        TEST_METHOD(ResetClearsEngineState)
        {
            FakeClock clock;
            bongo::BongoEngine engine(clock);
            engine.OnInput();
            engine.SetPaused(true);
            engine.Reset();

            Assert::AreEqual(0, engine.Slaps());
            Assert::AreEqual(static_cast<int>(bongo::Pose::idle), static_cast<int>(engine.CurrentPose()));
            Assert::IsTrue(engine.IsPaused());
        }

        TEST_METHOD(ComboMilestonesReturnSounds)
        {
            FakeClock clock;
            bongo::BongoEngine engine(clock);
            bongo::SlapResult result{};
            for (int i = 0; i < 10; ++i)
            {
                result = engine.OnInput();
                clock.Advance(std::chrono::milliseconds(100));
            }
            Assert::AreEqual(static_cast<int>(bongo::SoundId::combo), static_cast<int>(result.sound));

            for (int i = 10; i < 50; ++i)
            {
                result = engine.OnInput();
                clock.Advance(std::chrono::milliseconds(100));
            }
            Assert::AreEqual(static_cast<int>(bongo::SoundId::combo_high), static_cast<int>(result.sound));
        }
    };
}
