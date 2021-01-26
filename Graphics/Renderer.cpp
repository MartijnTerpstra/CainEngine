#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;

Renderer::Renderer(unique_ptr<API::IRenderer> renderer)
	: m_renderer(move(renderer))
{
}

Renderer::~Renderer()
{
}

uint32_t Renderer::ID() const
{
	return m_renderer->ID();
}

string Renderer::ShortName() const
{
	return m_renderer->ShortName();
}

string Renderer::Name() const
{
	return m_renderer->Name();
}

void Renderer::Init(flag<RendererInitFlags> initFlags)
{
	m_renderer->Init(initFlags);
}

void Renderer::Exit()
{
	m_renderer->Exit();
}

void Renderer::Flush()
{
	m_renderer->Flush();
}

void Renderer::SetMainWindow(const Common::RefPtr<Platform::IWindow>& mainWindow, const optional<SwapChainCreationSettings>& creationSettings)
{
	m_renderer->SetMainWindow(mainWindow, creationSettings);
}

void Renderer::HandleWindowResize()
{
	m_renderer->HandleWindowResize();
}

bool Renderer::HasFeature(RendererFeature feature) const
{
	return m_renderer->HasFeature(feature);
}

API::IDisplaySettings& CainEngine::Graphics::Renderer::DisplaySettings()
{
	return m_renderer->DisplaySettings();
}

API::IRenderer* Renderer::Implementation()
{
	return m_renderer.get();
}

Viewport Renderer::GetViewport() const
{
	return Viewport{ float2::zero, float2(m_renderer->GetBackBufferSize()) };
}