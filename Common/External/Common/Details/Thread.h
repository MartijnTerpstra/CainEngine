#pragma once

namespace Common {

class TaskManager;

namespace _Details {

class Thread
{
	friend class TaskManager;
public:
	Thread(TaskManager* manager);
	~Thread();

private:
	// For TaskManager
	
	void Run(std::promise<void>&& promise, std::function<void()>&& job);

	void Stop();

	void Wait();

private:
	// Internal functionality

	void ThreadProc();

private:
	// Member variables

	std::function<void()> m_job;
	std::promise<void> m_promise;
	bool m_run = false;
	bool m_quit = false;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	TaskManager* const m_manager;
	unique_ptr<std::thread> m_thread;
};

}

}