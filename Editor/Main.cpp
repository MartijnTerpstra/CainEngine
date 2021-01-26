#include "Precomp.h"

#include "ShaderSourceManager.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Editor;

int main()
{
	ShaderSourceManager manager;

	if (!manager.CompileShaders())
	{
		Common::FatalError("Error compiling shaders");
	}
}