#pragma once

namespace CainEngine::Editor::ShaderCompiler {

std::unique_ptr<ICompiler> createDX11Compiler();

std::unique_ptr<ICompiler> createDX12Compiler();

} // namespace CainEngine::Editor::ShaderCompiler