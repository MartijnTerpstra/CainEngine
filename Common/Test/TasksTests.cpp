#include <Common.h>

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

using namespace ::CainEngine;
using namespace ::CainEngine::Common;

TEST(TaskManager, RunExecutesJobAndFutureBecomesReady)
{
	TaskManager manager;
	std::atomic<bool> ran{false};

	auto future = manager.Run([&]() { ran = true; });
	future.wait();

	EXPECT_TRUE(ran.load());
}

TEST(TaskManager, RunExecutesMultipleJobsConcurrently)
{
	TaskManager manager;
	constexpr int jobCount = 8;
	std::atomic<int> completed{0};

	std::vector<std::future<void>> futures;
	for(int i = 0; i < jobCount; ++i)
	{
		futures.push_back(manager.Run([&]() { ++completed; }));
	}

	for(auto& future : futures)
		future.wait();

	EXPECT_EQ(jobCount, completed.load());
}

TEST(TaskManager, ConstructingWithInitialThreadCountStillRunsJobs)
{
	// Thread count isn't observable through the public API; this just checks
	// that pre-warming worker threads doesn't break job execution.
	TaskManager manager(4);
	std::atomic<bool> ran{false};

	manager.Run([&]() { ran = true; }).wait();

	EXPECT_TRUE(ran.load());
}

TEST(TaskManager, JobsRunOnADifferentThreadThanTheCaller)
{
	TaskManager manager;
	const auto callerThreadId = std::this_thread::get_id();
	std::thread::id jobThreadId;

	manager.Run([&]() { jobThreadId = std::this_thread::get_id(); }).wait();

	EXPECT_NE(callerThreadId, jobThreadId);
}

TEST(TaskManager, DestructorWaitsForOutstandingJobsToFinish)
{
	std::atomic<bool> ran{false};
	{
		TaskManager manager;
		manager.Run([&]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			ran = true;
		});
	} // destructor should block until the job completes

	EXPECT_TRUE(ran.load());
}

TEST(TaskManager, RunAfterPreviousJobCompletedReusesTheManager)
{
	TaskManager manager;

	manager.Run([]() {}).wait();

	std::atomic<bool> ran{false};
	manager.Run([&]() { ran = true; }).wait();

	EXPECT_TRUE(ran.load());
}
