#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;

Renderer::Renderer(std::unique_ptr<API::IRenderer> renderer)
	: m_renderer(std::move(renderer))
{
}

Renderer::~Renderer()
{
}

uint32_t Renderer::id() const
{
	return m_renderer->id();
}

std::string Renderer::shortName() const
{
	return m_renderer->shortName();
}

std::string Renderer::name() const
{
	return m_renderer->name();
}

void Renderer::init(flag<RendererInitFlags> initFlags)
{
	m_renderer->init(initFlags);
}

void Renderer::exit()
{
	m_renderer->exit();
}

void Renderer::flush()
{
	m_renderer->flush();
}

void Renderer::setMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow, const std::optional<SwapChainCreationSettings>& creationSettings)
{
	m_renderer->setMainWindow(mainWindow, creationSettings);
}

void Renderer::handleWindowResize()
{
	m_renderer->handleWindowResize();
}

bool Renderer::hasFeature(RendererFeature feature) const
{
	return m_renderer->hasFeature(feature);
}

API::IDisplaySettings& CainEngine::Graphics::Renderer::displaySettings()
{
	return m_renderer->displaySettings();
}

API::IRenderer* Renderer::implementation()
{
	return m_renderer.get();
}

Viewport Renderer::getViewport() const
{
	return Viewport{ float2::zero, float2(m_renderer->getBackBufferSize()) };
}