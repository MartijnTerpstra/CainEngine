#pragma once

namespace CainEngine::Graphics {

class Factory final
{
public:
	// ctor & dtor

	Factory(Renderer& renderer);
	~Factory();

	pair<int32_t, API::ITexture*> LoadTexture(Common::Source& source);

private:
	API::IFactory* const m_factory;
	API::IRenderer* const m_renderer;

};

}