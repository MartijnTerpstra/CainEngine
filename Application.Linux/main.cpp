#include <Platform.Xorg.h>

#include <chrono>
#include <thread>

using namespace ::CainEngine;

// Minimal Linux launcher: opens a window via the Xorg backend and pumps its
// event loop until closed. No rendering yet - once a Vulkan swap chain can be
// created from an IXorgWindow this is where it gets initialized and driven.
//
// Once a Platform.Wayland backend exists, this is where the two would be
// chosen between (e.g. try Wayland, fall back to Xorg).

class Application final : public Platform::ClientInterfaces::IWindowEventListener,
						   public std::enable_shared_from_this<Application>
{
public:
	explicit Application(const RefPtr<Platform::ICoreFactory>& platform)
		: m_platformFactory(platform)
	{ }

	void Run()
	{
		m_mainWindow = m_platformFactory->CreateNewWindow("Main Window", uint2(1280, 720),
			Platform::WindowType::Default, Platform::WindowFlags::Default, shared_from_this());

		m_mainWindow->Show();

		while (m_mainWindow->IsShown())
		{
			m_mainWindow->Redraw();

			m_mainWindow->HandleEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		m_mainWindow = nullptr;
	}

private:
	// IWindowEventListener overrides

	void OnRedraw(Platform::IWindow* window) override
	{
		// TODO: render a frame via Graphics::Vulkan once it's wired up.
	}

private:
	// Member variables

	RefPtr<Platform::IWindow> m_mainWindow;

	const RefPtr<Platform::ICoreFactory> m_platformFactory;
};

int main()
{
	auto platform = Platform::Xorg::CreateInstance();

	auto application = std::make_shared<Application>(platform);

	application->Run();
}
