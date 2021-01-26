#pragma once

#include <Platform.h>
#include <Graphics.h>

namespace CainEngine {
namespace Graphics {
namespace DX12 {

unique_ptr<Implementation::IRenderer> CreateInstance();

}; // namespace DX12
}; // namespace Graphics
}; // namespace CainEngine