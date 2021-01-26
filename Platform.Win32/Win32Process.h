#pragma once

namespace CainEngine {
namespace Platform {
namespace Internal {

class Win32Process final : public IProcess
{
	friend class Common::RefPtr<Win32Process>;
private:
	// ctor & dtor

	Win32Process(HANDLE processHandle);
	~Win32Process();

	COMMON_DECLARE_NON_COPY(Win32Process);

public:
	// Creation

	static vector<RefPtr<IProcess>> GetProcesses(const string& name);
	static RefPtr<IProcess> GetProcess(uint id);
	static RefPtr<IProcess> GetCurrentProcess();
	static RefPtr<IProcess> CreateNewProcess(const string& path, const string& commandLine, const string& workingDirectory);

public:
	// IProcess overrides

	string GetName() const override;
	uint GetID() const override;
	uint GetSessionID() const override;
	std::chrono::time_point<std::chrono::system_clock> GetCreationTime() const override;

private:
	// BaseObject overrides

	virtual void * _As(uint64_t) const override;

private:
	// Member variables

	const HANDLE m_process;

}; // class Win32Process

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine