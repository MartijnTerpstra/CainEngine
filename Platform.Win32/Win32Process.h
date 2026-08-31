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

	static std::vector<RefPtr<IProcess>> getProcesses(const std::string& name);
	static RefPtr<IProcess> getProcess(uint32_t id);
	static RefPtr<IProcess> getCurrentProcess();
	static RefPtr<IProcess> createNewProcess(const std::string& path,
		const std::string& commandLine, const std::string& workingDirectory);

public:
	// IProcess overrides

	std::string getName() const override;
	uint32_t getId() const override;
	uint32_t getSessionId() const override;
	std::chrono::time_point<std::chrono::system_clock> getCreationTime() const override;

private:
	// BaseObject overrides

	virtual void* asImpl(uint64_t) const override;

private:
	// Member variables

	const HANDLE m_process;

}; // class Win32Process

}; // namespace Internal
}; // namespace Platform
}; // namespace CainEngine