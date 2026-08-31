#pragma once

namespace CainEngine::ResourceSystem {

class ResourceManager
{
public:
	// ctor & dtor

	ResourceManager();
	~ResourceManager();

public:
	// Main functionality

	void init();

	std::unique_ptr<Source> getResource(uuid id);

private:

};

}
