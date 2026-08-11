#pragma once

namespace CainEngine {

class Engine
{
public:
	// ctor & dtor

	Engine(std::unique_ptr<Graphics::Renderer> renderer, std::string contentDirectory);
	~Engine();

public:
	// Singleton functionality

	static Engine& Get();

public:
	// Main functionality

	/**
		Summary:
			Initializes the engine
	*/
	void Init(flag<Graphics::RendererInitFlags> initFlags);

	/**
		Summary:
			Removes all resources the engine
	*/
	void Exit();

	/**
		Summary:
			Sets the main target window
	*/
	void SetMainWindow(const RefPtr<Platform::IWindow>& mainWindow, const std::optional<Graphics::SwapChainCreationSettings>& creationSettings);

	/**
		Summary:
			Call this function when a window is resized
	*/
	void HandleResize();

	/**
		Summary:
			Renders a new frame
	*/
	void RenderFrame(const std::optional<Graphics::Viewport>& viewport);

	/**
		Summary:
			Returns the renderer
	*/
	Graphics::Renderer& GetRenderer();

	/**
		Summary:
			Returns the Camera Component Manager
	*/
	Graphics::CameraManager& GetCameraManager();

	/**
		Summary:
			Returns the Model Component Manager
	*/
	Graphics::ModelManager& GetModelManager();

	/**
		Summary:
			Returns the scene
	*/
	EntitySystem::Scene& GetScene();

private:
	// Internal functionality

	std::future<void> LoadShaders();

private:
	// Member variables

	const std::unique_ptr<Graphics::Renderer> m_renderer;
	Common::TaskManager m_taskManager;

	Graphics::CameraManager m_cameraManager;
	Graphics::ModelManager m_modelManager;
	EntitySystem::Scene m_scene;

	const std::string m_contentDirectory;

	static Engine* s_singleton;

}; // class Engine

};