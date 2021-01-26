#pragma once

#include <Platform.h>
#include <Graphics.h>

namespace CainEngine {
namespace Graphics {
namespace Vulkan {

unique_ptr<Implementation::IRenderer> CreateInstance();

}; // namespace Vulkan
}; // namespace Graphics
}; // namespace CainEngine