#pragma once

#include <Platform.h>
#include <Graphics.h>

namespace CainEngine {
namespace Graphics {
namespace Vulkan {

unique_ptr<Implementation::IRenderer> createInstance();

}; // namespace Vulkan
}; // namespace Graphics
}; // namespace CainEngine