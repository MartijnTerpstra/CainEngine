#include "Precomp.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Graphics;

Material::Material(API::VertexShader* vs, API::PixelShader* ps)
	: m_vertexShader(vs)
	, m_pixelShader(ps)
{ }

Material::~Material()
{ }

API::VertexShader* Material::vertexShader() const
{
	return m_vertexShader;
}

API::PixelShader* Material::pixelShader() const
{
	return m_pixelShader;
}