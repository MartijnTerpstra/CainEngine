#include <Engine.h>
#include <Platform.Win32.h>
#include <Graphics.DX11.h>
#include <EntitySystem.h>

using namespace ::CainEngine;

class Application final : public Platform::ClientInterfaces::IWindowEventListener, public std::enable_shared_from_this<Application>
{
public:

	Application(Engine& engine, const Common::RefPtr<CainEngine::Platform::ICoreFactory>& platform)
		: m_platformFactory(platform)
		, m_engine(engine)
	{
	}

	void Run()
	{
		m_engine.Init(make_flag(Graphics::RendererInitFlags::ApiDebug));

		m_mainWindow = m_platformFactory->CreateNewWindow("Main Window", uint2(1280, 720), Platform::WindowType::Default, Platform::WindowFlags::Default, shared_from_this());

		auto rect = m_mainWindow->GetClientRect();

		m_mainWindow->Show();

		m_engine.SetMainWindow(m_mainWindow, Graphics::SwapChainCreationSettings(Graphics::PixelFormat::UnormBGRA8, 144, false, 4));

		InitScene();

		std::chrono::high_resolution_clock clock;

		auto start = clock.now();

		while (m_mainWindow->IsShown())
		{
			m_mainWindow->Redraw();

			m_mainWindow->HandleEvents();

			UpdateScene(clock.now() - start);

			Sleep(25);
		}

		m_engine.GetScene().Clear();

		m_engine.SetMainWindow(nullptr, none);

		m_engine.Exit();

		m_mainWindow = nullptr;
	}

	void InitScene()
	{
		auto& scene = m_engine.GetScene();
		auto& cameraManager = m_engine.GetCameraManager();
		auto& modelManager = m_engine.GetModelManager();
		auto& renderer = m_engine.GetRenderer();

		m_camera = scene.Create(u8"Main camera");

		scene.GetTransform(m_camera).SetPosition(float3(0, 0, 10));
		//m_camera.AddComponent<Graphics::Camera>(Graphics::Camera(90, 1, 100));

		// just use clip space
		cameraManager.AddCamera(m_camera);
		cameraManager.SetOrthographicProjection(m_camera, float2(-1, -1), float2(1, 1), 0, 1);

		Graphics::VertexDataCreationInfo vertexData;

		vertexData.positions = { { -1, 1, 0 }, { 1, 1, 0 }, { 0, -1, 0 } };
		vertexData.uvs = { { 0, 0 }, { 1, 0 }, { 0.5f, 1 } };
		vertexData.normals = { { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 } };
		vertexData.topology = Graphics::PrimitiveTopology::TriangleList;
		vertexData.indices = { 0, 1, 2 };

		Graphics::Factory factory{ renderer };

		auto material = modelManager.CreateMaterial(renderer, Graphics::ShaderManager::GetVertexShader("VertexShader"), Graphics::ShaderManager::GetPixelShader("PixelShader"));

		auto model = modelManager.CreateModel(renderer, vertexData);

		model.second->AddSubMesh(0, 3, material.first);

		m_simpleTriangle = scene.Create(u8"Simple triangle");
		modelManager.AddEntity(model.first, scene, m_simpleTriangle);
		//m_simpleTriangle.AddComponent<shared_ptr<Graphics::Model>>(move(model));
	}

	void UpdateScene(std::chrono::nanoseconds ns)
	{
		/*float dt = float(ns.count() / (long double)1000000);

		auto& transform = m_scene.GetTransform(m_simpleTriangle);

		transform.SetEulerAngles(transform.EulerX() + degrees(5) * dt, 0, 0);*/
	}

private:


	// IWindowEventListener overrides

	void OnRedraw(Platform::IWindow* window) override
	{
		m_engine.RenderFrame(none);
	}

	void OnResize(Platform::IWindow* window, const uint2& newSize) override
	{
		m_engine.HandleResize();
	}

	void OnKeyDown(Platform::IWindow* window, Platform::KeyCodes keyCode, flag<Platform::KeyModifiers> modifiers)
	{
		switch (keyCode)
		{
		case CainEngine::Platform::KeyCodes::A:
		{
			auto fs = m_engine.GetRenderer().DisplaySettings().FullScreen();
			if (fs.isFullScreen)
			{
				m_engine.GetRenderer().DisplaySettings().SetFullScreen(false, fs.outputIndex);
			}
			else
			{
				m_engine.GetRenderer().DisplaySettings().SetFullScreen(true, fs.outputIndex);
			}
			return;
		}
		case CainEngine::Platform::KeyCodes::S:
		{
			auto ms = m_engine.GetRenderer().DisplaySettings().MultiSamplingCount();
			if (ms == 1)
			{
				m_engine.GetRenderer().DisplaySettings().SetMultiSamplingCount(4);
			}
			else
			{
				m_engine.GetRenderer().DisplaySettings().SetMultiSamplingCount(1);
			}
			return;
		}
		default:
			break;
		}
	}

private:
	// Member variables

	RefPtr<Platform::IWindow> m_mainWindow;

	Engine& m_engine;
	const RefPtr<Platform::ICoreFactory> m_platformFactory;

	EntitySystem::EntityID m_camera;
	EntitySystem::EntityID m_simpleTriangle;

};

void RunEngine(Engine& engine, const Common::RefPtr<CainEngine::Platform::ICoreFactory>& platform)
{
	auto application = make_shared<Application>(engine, platform);

	application->Run();
}

void InitEngine(const Common::RefPtr<CainEngine::Platform::ICoreFactory>& platform)
{
	Engine engine(make_unique<Graphics::Renderer>(Graphics::DX11::CreateInstance()), CONTENT_DIRECTORY);

	RunEngine(engine, platform);
}

extern void RunBenchmark();

int main()
{
	//RunBenchmark();
	//return 0;

	auto platform = Platform::Win32::CreateInstance();

	InitEngine(platform);
}