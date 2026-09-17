#include "CppUnitTest.h"

#include "BongoEngine.h"
#include "ComboTracker.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BongoCoreTests
{
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
			for (int i = 0; i < 29; ++i)
			{
				state = tracker.OnSlap(bongo::TimePoint{} + std::chrono::milliseconds(i * 100));
			}
			Assert::AreEqual(29, state.count);
			Assert::AreEqual(static_cast<int>(bongo::ComboColor::yellow), static_cast<int>(state.color));

			state = tracker.OnSlap(bongo::TimePoint{} + std::chrono::milliseconds(2900));
			Assert::AreEqual(30, state.count);
			Assert::AreEqual(static_cast<int>(bongo::ComboColor::orange), static_cast<int>(state.color));
			Assert::AreEqual(20, state.font_size);
		}
	};

	TEST_CLASS(BongoEngineTests)
	{
	public:
		TEST_METHOD(SlapsAlternatePosesAndReturnToIdle)
		{
			bongo::BongoEngine engine;
			const auto first = engine.OnInput(bongo::TimePoint{});
			const auto second = engine.OnInput(bongo::TimePoint{} + std::chrono::milliseconds(10));

			Assert::AreEqual(static_cast<int>(bongo::Pose::left), static_cast<int>(first.pose));
			Assert::AreEqual(static_cast<int>(bongo::Pose::right), static_cast<int>(second.pose));
			Assert::IsTrue(engine.Tick(bongo::TimePoint{} + std::chrono::milliseconds(109)));
            Assert::IsFalse(engine.Tick(bongo::TimePoint{} + std::chrono::milliseconds(110)));
			Assert::AreEqual(static_cast<int>(bongo::Pose::idle), static_cast<int>(engine.CurrentPose()));
		}

		TEST_METHOD(MaxSlapsCapsCounter)
		{
			bongo::BongoEngine engine({2});
			engine.OnInput(bongo::TimePoint{});
			engine.OnInput(bongo::TimePoint{} + std::chrono::milliseconds(10));
			const auto result = engine.OnInput(bongo::TimePoint{} + std::chrono::milliseconds(20));

			Assert::AreEqual(2, result.slaps);
		}

		TEST_METHOD(PausedEngineIgnoresInput)
		{
			bongo::BongoEngine engine;
			engine.SetPaused(true);
			const auto result = engine.OnInput(bongo::TimePoint{});

			Assert::IsFalse(result.accepted);
			Assert::AreEqual(0, result.slaps);
			Assert::AreEqual(static_cast<int>(bongo::Pose::idle), static_cast<int>(result.pose));
		}

		TEST_METHOD(ComboMilestonesReturnSounds)
		{
			bongo::BongoEngine engine;
			bongo::SlapResult result{};
			for (int i = 0; i < 10; ++i)
			{
				result = engine.OnInput(bongo::TimePoint{} + std::chrono::milliseconds(i * 100));
			}
			Assert::AreEqual(static_cast<int>(bongo::SoundId::combo), static_cast<int>(result.sound));

			for (int i = 10; i < 50; ++i)
			{
				result = engine.OnInput(bongo::TimePoint{} + std::chrono::milliseconds(i * 100));
			}
			Assert::AreEqual(static_cast<int>(bongo::SoundId::combo_high), static_cast<int>(result.sound));
		}
	};
}
