#include "Precomp.h"

using namespace ::CainEngine;

Engine* Engine::s_singleton = nullptr;

Engine::Engine(unique_ptr<Graphics::Renderer> renderer, string contentDirectory)
	: m_renderer(move(renderer)),
	m_contentDirectory(move(contentDirectory))
{
	m_cameraManager.AttachCallbacks(m_scene);
	m_modelManager.AttachCallbacks(m_scene);
}

Engine::~Engine()
{
}

Engine& Engine::Get()
{
	return *s_singleton;
}

void Engine::Init(flag<Graphics::RendererInitFlags> initFlags)
{
	m_renderer->Init(initFlags);

	auto shaderLoadingTask = LoadShaders();

	shaderLoadingTask.wait();
}

void Engine::Exit()
{
	// Flush all commands to leave an inactive graphics pipeline
	m_renderer->Flush();

	Graphics::ShaderManager::Exit();

	m_renderer->Exit();
}

void Engine::SetMainWindow(const RefPtr<Platform::IWindow>& mainWindow, const optional<Graphics::SwapChainCreationSettings>& creationSettings)
{
	m_renderer->SetMainWindow(mainWindow, creationSettings);
}

void Engine::HandleResize()
{
	m_renderer->HandleWindowResize();
}

void Engine::RenderFrame(const optional<Graphics::Viewport>& viewport)
{
	m_renderer->RenderFrame(m_scene, m_cameraManager, m_modelManager, viewport);
}

Graphics::Renderer& Engine::GetRenderer()
{
	return *m_renderer;
}

Graphics::CameraManager& Engine::GetCameraManager()
{
	return m_cameraManager;
}

Graphics::ModelManager& Engine::GetModelManager()
{
	return m_modelManager;
}

EntitySystem::Scene& Engine::GetScene()
{
	return m_scene;
}

std::future<void> Engine::LoadShaders()
{
	return m_taskManager.Run([this]()
	{
		Common::FileSource shaderFile(m_contentDirectory + "/Shaders/" + m_renderer->ShortName() + ".shaders");

		Graphics::ShaderManager::Init(m_renderer.get(), shaderFile);
	});
}