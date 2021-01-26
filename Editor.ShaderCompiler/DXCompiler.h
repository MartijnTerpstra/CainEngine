#pragma once

namespace CainEngine::Editor::ShaderCompiler {

unique_ptr<ICompiler> CreateDX11Compiler();

unique_ptr<ICompiler> CreateDX12Compiler();

}