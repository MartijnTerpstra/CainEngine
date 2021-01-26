#pragma once

namespace Common {

class TaskManager
{
	friend class _Details::Thread;
public:
	// ctor & dtor
	TaskManager(uint32_t initialThreadCount = 0);
	~TaskManager();

public:
	// Main functionality

	std::future<void> Run(std::function<void()> job);

private:
	// For Task

	void TaskCompleted(_Details::Thread* thread);

private:
	std::mutex m_mutex;
	std::vector<_Details::Thread*> m_waitingThreads;
	std::vector<unique_ptr<_Details::Thread>> m_allThreads;
};

}