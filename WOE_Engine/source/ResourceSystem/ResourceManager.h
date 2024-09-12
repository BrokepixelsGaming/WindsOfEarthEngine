#pragma once
#include <unordered_map>

#include "tools/Utilities/StringId.h"

class Resource;

struct ResourceEntry
{
	Resource* resource = nullptr;
	int refCount = 0;
};
class ResourceManager
{
	std::unordered_map<StringId, ResourceEntry, StringIdStdHashFunctor> m_resources;

public:
	bool Init();
	bool FlushAllResources();
	Resource* Cache(const StringId& id);
	void Flush(const StringId& id);
private:
	Resource* _LoadResource(const StringId& id);
	Resource* CreateResourceFromType(const StringId& id);
};