#include "Precomp.h"

#include "ShaderSourceManager.h"

using namespace ::CainEngine;
using namespace ::CainEngine::Editor;

int main()
{
	ShaderSourceManager manager;

	if (!manager.compileShaders())
	{
		Common::fatalError("Error compiling shaders");
	}
}