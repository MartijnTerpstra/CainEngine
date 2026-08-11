#pragma once

namespace CainEngine::Editor::ShaderCompiler {

std::unique_ptr<ICompiler> CreateDX11Compiler();

std::unique_ptr<ICompiler> CreateDX12Compiler();

}