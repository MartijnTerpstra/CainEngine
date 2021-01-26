#pragma once

#include <Common.h>
#include <Platform.h>
#include <EntitySystem.h>

// Forward declarations
namespace CainEngine {
namespace Graphics {

// Defined classes
class CameraManager;
class Material;
class Model;
class ModelManager;
class Renderer;

// Defined structs
struct Viewport;

namespace API {

class PixelShader;
class VertexShader;
class ComputeShader;
class GeometryShader;
class HullShader;
class DomainShader;

class VertexData;

class IBuffer;
class IFactory;
class IRenderContext;
class IResource;
class IResourceGroup;
class ITexture;

};

}; // namespace Graphics
}; // namespace CainEngine

#include "Graphics/Enums.h"
#include "Graphics/Structs.h"

#include "Graphics/API/Enums.h"
#include "Graphics/API/Structs.h"
#include "Graphics/API/IResource.h"
#include "Graphics/API/IBuffer.h"
#include "Graphics/API/ITexture.h"
#include "Graphics/API/IFactory.h"
#include "Graphics/API/IDisplaySettings.h"
#include "Graphics/API/IRenderer.h"
#include "Graphics/API/IRenderContext.h"

#include "Graphics/Material.h"
#include "Graphics/Model.h"

#include "Graphics/Factory.h"

#include "Graphics/CameraManager.h"
#include "Graphics/ModelManager.h"

#include "Graphics/Implementation/FrameData.h"
#include "Graphics/Renderer.h"

#include "Graphics/ShaderManager.h"