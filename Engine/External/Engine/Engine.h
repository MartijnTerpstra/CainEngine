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

	static Engine& get();

public:
	// Main functionality

	/**
		Summary:
			Initializes the engine
	*/
	void init(flag<Graphics::RendererInitFlags> initFlags);

	/**
		Summary:
			Removes all resources the engine
	*/
	void exit();

	/**
		Summary:
			Sets the main target window
	*/
	void setMainWindow(const RefPtr<Platform::IWindow>& mainWindow,
		const std::optional<Graphics::SwapChainCreationSettings>& creationSettings);

	/**
		Summary:
			Call this function when a window is resized
	*/
	void handleResize();

	/**
		Summary:
			Renders a new frame
	*/
	void renderFrame(const std::optional<Graphics::Viewport>& viewport);

	/**
		Summary:
			Returns the renderer
	*/
	Graphics::Renderer& getRenderer();

	/**
		Summary:
			Returns the Camera Component Manager
	*/
	Graphics::CameraManager& getCameraManager();

	/**
		Summary:
			Returns the Model Component Manager
	*/
	Graphics::ModelManager& getModelManager();

	/**
		Summary:
			Returns the scene
	*/
	EntitySystem::Scene& getScene();

private:
	// Internal functionality

	std::future<void> loadShaders();

private:
	// Member variables

	const std::unique_ptr<Graphics::Renderer> m_renderer;
	Common::TaskManager m_taskManager;

	EntitySystem::Scene m_scene;
	Graphics::CameraManager m_cameraManager;
	Graphics::ModelManager m_modelManager;

	const std::string m_contentDirectory;

	static Engine* s_singleton;

}; // class Engine

}; // namespace CainEngine