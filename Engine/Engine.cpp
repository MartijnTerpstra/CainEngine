#include "Precomp.h"

using namespace ::CainEngine;

Engine* Engine::s_singleton = nullptr;

Engine::Engine(std::unique_ptr<Graphics::Renderer> renderer, std::string contentDirectory)
	: m_renderer(std::move(renderer))
	, m_contentDirectory(std::move(contentDirectory))
	, m_cameraManager(m_scene)
{
	m_modelManager.attachCallbacks(m_scene);
}

Engine::~Engine()
{ }

Engine& Engine::get()
{
	return *s_singleton;
}

void Engine::init(flag<Graphics::RendererInitFlags> initFlags)
{
	m_renderer->init(initFlags);

	auto shaderLoadingTask = loadShaders();

	shaderLoadingTask.wait();
}

void Engine::exit()
{
	// Flush all commands to leave an inactive graphics pipeline
	m_renderer->flush();

	Graphics::ShaderManager::exit();

	m_renderer->exit();
}

void Engine::setMainWindow(const RefPtr<Platform::IWindow>& mainWindow,
	const std::optional<Graphics::SwapChainCreationSettings>& creationSettings)
{
	m_renderer->setMainWindow(mainWindow, creationSettings);
}

void Engine::handleResize()
{
	m_renderer->handleWindowResize();
}

void Engine::renderFrame(const std::optional<Graphics::Viewport>& viewport)
{
	m_renderer->renderFrame(m_scene, m_cameraManager, m_modelManager, viewport);
}

Graphics::Renderer& Engine::getRenderer()
{
	return *m_renderer;
}

Graphics::CameraManager& Engine::getCameraManager()
{
	return m_cameraManager;
}

Graphics::ModelManager& Engine::getModelManager()
{
	return m_modelManager;
}

EntitySystem::Scene& Engine::getScene()
{
	return m_scene;
}

std::future<void> Engine::loadShaders()
{
	return m_taskManager.run([this]() {
		Common::FileSource shaderFile(
			m_contentDirectory + "/Shaders/" + m_renderer->shortName() + ".shaders");

		Graphics::ShaderManager::init(m_renderer.get(), shaderFile);
	});
}