#pragma once

#include <Platform.h>
#include <Graphics.h>

namespace CainEngine {
namespace Graphics {
namespace DX12 {

std::unique_ptr<API::IRenderer> createInstance();

}; // namespace DX12
}; // namespace Graphics
}; // namespace CainEngine