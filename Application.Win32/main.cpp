#include <Engine.h>
#include <Platform.Win32.h>
#include <Graphics.DX11.h>
#include <EntitySystem.h>

using namespace ::CainEngine;

class Application final : public Platform::ClientInterfaces::IWindowEventListener,
						  public std::enable_shared_from_this<Application>
{
public:
	Application(Engine& engine, const Common::RefPtr<CainEngine::Platform::ICoreFactory>& platform)
		: m_platformFactory(platform)
		, m_engine(engine)
	{ }

	void run()
	{
		m_engine.init(make_flag(Graphics::RendererInitFlags::ApiDebug));

		m_mainWindow = m_platformFactory->createNewWindow("Main Window", uint2(1280, 720),
			Platform::WindowType::Default, Platform::WindowFlags::Default, shared_from_this());

		auto rect = m_mainWindow->getClientRect();

		m_mainWindow->show();

		m_engine.setMainWindow(m_mainWindow,
			Graphics::SwapChainCreationSettings(Graphics::PixelFormat::UnormBGRA8, 144, false, 4));

		initScene();

		std::chrono::high_resolution_clock clock;

		auto start = clock.now();

		while(m_mainWindow->isShown())
		{
			m_mainWindow->redraw();

			m_mainWindow->handleEvents();

			updateScene(clock.now() - start);

			Sleep(25);
		}

		m_engine.getScene().clear();

		m_engine.setMainWindow(nullptr, std::nullopt);

		m_engine.exit();

		m_mainWindow = nullptr;
	}

	void initScene()
	{
		auto& scene = m_engine.getScene();
		auto& cameraManager = m_engine.getCameraManager();
		auto& modelManager = m_engine.getModelManager();
		auto& renderer = m_engine.getRenderer();

		m_camera = scene.create("Main camera");

		scene.getTransform(m_camera).setPosition(float3(0, 0, 10));
		// m_camera.AddComponent<Graphics::Camera>(Graphics::Camera(90, 1, 100));

		// just use clip space
		cameraManager.addCamera(m_camera);
		cameraManager.setOrthographicProjection(m_camera, float2(-1, -1), float2(1, 1), 0, 1);

		Graphics::VertexDataCreationInfo vertexData;

		vertexData.positions = { { -1, 1, 0 }, { 1, 1, 0 }, { 0, -1, 0 } };
		vertexData.uvs = { { 0, 0 }, { 1, 0 }, { 0.5f, 1 } };
		vertexData.normals = { { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 } };
		vertexData.topology = Graphics::PrimitiveTopology::TriangleList;
		vertexData.indices = { 0, 1, 2 };

		Graphics::Factory factory{ renderer };

		auto material = modelManager.createMaterial(renderer,
			Graphics::ShaderManager::getVertexShader("VertexShader"),
			Graphics::ShaderManager::getPixelShader("PixelShader"));

		auto model = modelManager.createModel(renderer, vertexData);

		model.second->addSubMesh(0, 3, material.first);

		m_simpleTriangle = scene.create("Simple triangle");
		modelManager.addEntity(model.first, scene, m_simpleTriangle);
		// m_simpleTriangle.AddComponent<shared_ptr<Graphics::Model>>(move(model));
	}

	void updateScene(std::chrono::nanoseconds ns)
	{
		/*float dt = float(ns.count() / (long double)1000000);

		auto& transform = m_scene.GetTransform(m_simpleTriangle);

		transform.SetEulerAngles(transform.EulerX() + degrees(5) * dt, 0, 0);*/
		Graphics::ShaderManager::getPixelShader("PixelShader");
	}

private:
	// IWindowEventListener overrides

	void onRedraw(Platform::IWindow* window) override
	{
		m_engine.renderFrame(std::nullopt);
	}

	void onResize(Platform::IWindow* window, const uint2& newSize) override
	{
		m_engine.handleResize();
	}

	void OnKeyDown(Platform::IWindow* window, Platform::KeyCodes keyCode,
		flag<Platform::KeyModifiers> modifiers)
	{
		switch(keyCode)
		{
		case CainEngine::Platform::KeyCodes::A: {
			auto fs = m_engine.getRenderer().displaySettings().fullScreen();
			if(fs.isFullScreen)
			{
				m_engine.getRenderer().displaySettings().setFullScreen(false, fs.outputIndex);
			}
			else
			{
				m_engine.getRenderer().displaySettings().setFullScreen(true, fs.outputIndex);
			}
			return;
		}
		case CainEngine::Platform::KeyCodes::S: {
			auto ms = m_engine.getRenderer().displaySettings().multiSamplingCount();
			if(ms == 1)
			{
				m_engine.getRenderer().displaySettings().setMultiSamplingCount(4);
			}
			else
			{
				m_engine.getRenderer().displaySettings().setMultiSamplingCount(1);
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

void runEngine(Engine& engine, const Common::RefPtr<CainEngine::Platform::ICoreFactory>& platform)
{
	auto application = std::make_shared<Application>(engine, platform);

	application->run();
}

void initEngine(const Common::RefPtr<CainEngine::Platform::ICoreFactory>& platform)
{
	Engine engine(
		std::make_unique<Graphics::Renderer>(Graphics::DX11::createInstance()), CONTENT_DIRECTORY);

	runEngine(engine, platform);
}

extern void runBenchmark();

int main()
{
	// RunBenchmark();
	// return 0;

	auto platform = Platform::Win32::createInstance();

	initEngine(platform);
}