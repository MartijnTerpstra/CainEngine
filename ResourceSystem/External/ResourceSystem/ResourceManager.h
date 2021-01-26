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

	void Init();

	unique_ptr<Source> GetResource(uuid id);

private:

};

}