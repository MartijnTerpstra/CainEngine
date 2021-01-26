#include "Precomp.h"

using namespace Common;

TaskManager::TaskManager(uint32_t initialThreadCount)
{
	m_allThreads.reserve(initialThreadCount);
	m_waitingThreads.reserve(initialThreadCount);

	for (uint32_t i = 0; i < initialThreadCount; ++i)
	{
		m_allThreads.push_back(make_unique<_Details::Thread>(this));
		m_waitingThreads.push_back(m_allThreads.back().get());
	}
}

TaskManager::~TaskManager()
{
	for (auto& task : m_allThreads)
	{
		task->Stop();
	}

	for (auto& task : m_allThreads)
	{
		task->Wait();
	}

	m_allThreads.clear();
}

std::future<void> TaskManager::Run(std::function<void()> job)
{
	std::lock_guard<std::mutex> l(m_mutex);

	if (m_waitingThreads.empty())
	{
		m_allThreads.push_back(make_unique<_Details::Thread>(this));
		m_waitingThreads.push_back(m_allThreads.back().get());
	}

	auto retval = m_waitingThreads.back();
	m_waitingThreads.pop_back();

	std::promise<void> promise;

	auto future = promise.get_future();

	retval->Run(move(promise), move(job));

	return future;
}

void TaskManager::TaskCompleted(_Details::Thread* thread)
{
	std::lock_guard<std::mutex> l(m_mutex);

	m_waitingThreads.push_back(thread);
}

_Details::Thread::Thread(TaskManager* manager)
	: m_manager(manager)
{
	m_thread = make_unique<std::thread>([this]() { ThreadProc(); });
}

_Details::Thread::~Thread()
{
}

void _Details::Thread::Run(std::promise<void>&& promise, std::function<void()>&& job)
{
	m_job = move(job);
	m_promise = move(promise);

	{
		std::lock_guard<std::mutex> l(m_mutex);
		m_run = true;
	}
	m_cv.notify_one();
}

void _Details::Thread::Stop()
{
	{
		std::lock_guard<std::mutex> l(m_mutex);
		m_quit = true;
	}
	m_cv.notify_one();
}

void _Details::Thread::Wait()
{
	m_thread->join();
	m_thread.reset();
}

void _Details::Thread::ThreadProc()
{
	while (true)
	{
		std::unique_lock<std::mutex> l(m_mutex);
		m_cv.wait(l, [this]() { return m_run || m_quit; });

		m_run = false;

		if (m_quit)
			return;

		m_job();
		m_promise.set_value();

		l.unlock();
		m_manager->TaskCompleted(this);
	}
}