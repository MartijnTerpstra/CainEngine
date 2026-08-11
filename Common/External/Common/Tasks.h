#pragma once

namespace CainEngine::Common {

class TaskManager
{
	friend class Details::Thread;

public:
	// ctor & dtor
	TaskManager(uint32_t initialThreadCount = 0);
	~TaskManager();

public:
	// Main functionality

	std::future<void> Run(std::function<void()> job);

private:
	// For Task

	void TaskCompleted(Details::Thread* thread);

private:
	std::mutex m_mutex;
	std::vector<Details::Thread*> m_waitingThreads;
	std::vector<std::unique_ptr<Details::Thread>> m_allThreads;
};

} // namespace CainEngine::Common