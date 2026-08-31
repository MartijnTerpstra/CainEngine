#include "Precomp.h"

using namespace CainEngine::Common;

TaskManager::TaskManager(uint32_t initialThreadCount)
{
	m_allThreads.reserve(initialThreadCount);
	m_waitingThreads.reserve(initialThreadCount);

	for (uint32_t i = 0; i < initialThreadCount; ++i)
	{
		m_allThreads.push_back(std::make_unique<Details::Thread>(this));
		m_waitingThreads.push_back(m_allThreads.back().get());
	}
}

TaskManager::~TaskManager()
{
	for (auto& task : m_allThreads)
	{
		task->stop();
	}

	for (auto& task : m_allThreads)
	{
		task->wait();
	}

	m_allThreads.clear();
}

std::future<void> TaskManager::run(std::function<void()> job)
{
	std::lock_guard<std::mutex> l(m_mutex);

	if (m_waitingThreads.empty())
	{
		m_allThreads.push_back(std::make_unique<Details::Thread>(this));
		m_waitingThreads.push_back(m_allThreads.back().get());
	}

	auto retval = m_waitingThreads.back();
	m_waitingThreads.pop_back();

	std::promise<void> promise;

	auto future = promise.get_future();

	retval->run(std::move(promise), std::move(job));

	return future;
}

void TaskManager::taskCompleted(Details::Thread* thread)
{
	std::lock_guard<std::mutex> l(m_mutex);

	m_waitingThreads.push_back(thread);
}

Details::Thread::Thread(TaskManager* manager)
	: m_manager(manager)
{
	m_thread = std::make_unique<std::thread>([this]() { threadProc(); });
}

Details::Thread::~Thread()
{
}

void Details::Thread::run(std::promise<void>&& promise, std::function<void()>&& job)
{
	m_job = std::move(job);
	m_promise = std::move(promise);

	{
		std::lock_guard<std::mutex> l(m_mutex);
		m_run = true;
	}
	m_cv.notify_one();
}

void Details::Thread::stop()
{
	{
		std::lock_guard<std::mutex> l(m_mutex);
		m_quit = true;
	}
	m_cv.notify_one();
}

void Details::Thread::wait()
{
	m_thread->join();
	m_thread.reset();
}

void Details::Thread::threadProc()
{
	while (true)
	{
		std::unique_lock<std::mutex> l(m_mutex);
		m_cv.wait(l, [this]() { return m_run || m_quit; });

		// A job that was already queued (m_run) must still run to completion
		// even if m_quit raced ahead and got set too - e.g. TaskManager's
		// destructor calls Stop() right after Run() queues a job, often
		// before this thread even gets scheduled. Only treat m_quit as a
		// reason to exit once there's no pending job left to run.
		if (!m_run)
			return;

		m_run = false;

		l.unlock();

		m_job();
		m_promise.set_value();

		m_manager->taskCompleted(this);
	}
}