#include "ResourceManager.h"

//#include <BleachLua/LuaState.h>
//#include <BleachLua/LuaVar.h>
//#include <BleachLua/LuaFunction.h>

#include "EngineInterface/EngineSingleton.h"
#include "Resource.h"
#include "ComponentSystem/Audio.h"


bool ResourceManager::Init()
{
	return true;
}

bool ResourceManager::FlushAllResources()
{
	if (!m_resources.empty()) 
	{
		for (auto& resource : m_resources)
		{
			resource.second.refCount = 0;
			BLEACH_DELETE(resource.second.resource);
			resource.second.resource = nullptr;
		}
		m_resources.clear();
	}
	
	return true;
}

Resource* ResourceManager::Cache(const StringId& id)
{
	// check to see if we already have a entry for this Id.
	auto findIt = m_resources.find(id);
	ResourceEntry entry;
	// No entry, so we try to load the resource.
	if (findIt == m_resources.end())
	{
		// try to load and then check for failure. 
		auto resource = _LoadResource(id);
		if (!resource)
		{
			//auto& luaState = EngineSingleton::Get()->GetLuaState();
			////entry Lua print string here. 
			//BleachLua::LuaFunction<void> PrintError = luaState.GetGlobal<BleachLua::LuaVar>("PrintError");
			//PrintError("Failed to load:: ");
			//PrintError(id.Get().c_str());
			//return nullptr; // we failed. 
		}
		// We have loaded the resource, so try to create a new entry. 
		entry.resource = resource;
		//m_resources.emplace(id, entry);
	}
	entry.refCount++;
	m_resources.emplace(id, entry);
	return entry.resource;
}

void ResourceManager::Flush(const StringId& id)
{
	auto findIt = m_resources.find(id);
	ResourceEntry entry;
	// No entry, so we try to load the resource.
	if (findIt == m_resources.end())
	{
		// Trying to flush a resource that is already dead or never existed. 
		//auto& luaState = EngineSingleton::Get()->GetLuaState();
		//luaState.DoString("Failure to flush Resource :: ");
		//luaState.DoString(id.Get().c_str());
		return;
	}
	findIt->second.refCount--;
	if (findIt->second.refCount <= 0)
	{
		BLEACH_DELETE(findIt->second.resource); // Destroy the resource correctly based on type.  has to be valid
		findIt->second.resource = nullptr; // set the pointer to null.
		m_resources.erase(findIt); // actually erase the resource entry. 
	}
}

Resource* ResourceManager::_LoadResource(const StringId& id)
{
	
	//// check to see if the file exists
	//if not file exists for id
	//auto findIt = m_resources.find(id);
	//if(findIt == m_resources.end())
	//	error("No file")
	//	return null

	//	// load the file as a raw byte stream
	//	rawData = load resource from file for id

	//	// tell the resource to load itself
		//auto resource =  // factory
	//	if !resource.Load(rawData)
	//		return null

	//		// return the thing
	//		return resource
	return CreateResourceFromType(id);
}

///////////////////////////////////////////////////////////////
//// TODO(Shilo): This should be extracted into a Factory for Resource types. 
///////////////////////////////////////////////////////////////
Resource* ResourceManager::CreateResourceFromType(const StringId& id)
{
	auto const pos = id.Get().find_last_of('.');
	const auto type = id.Get().substr(pos + 1);

	if (type == "txt")
	{
		// here we build txt map. 
		auto pMap = BLEACH_NEW(TileMap);
		pMap->Load(id);
		return pMap;
	}
	if (type == "zip")
	{
		// here we unzip 
	}
	if (type == "xml")
	{
		// here we deal with XML
	}
	if (type == "tmx")
	{
		// Load Tiled data
		auto pMap = BLEACH_NEW(TileMap);
		pMap->Load(id);
		return pMap;
	}
	if (type == "mp3")
	{
		auto pAudio = BLEACH_NEW(Audio);
		pAudio->Load(id.Get().c_str());
		return pAudio;
	}

	// Here we failed to create anything have have returned a invalid pointer. 
	//auto& luaState = EngineSingleton::Get()->GetLuaState();
	//luaState.DoString("Failure to create Resource :: ");
	//luaState.DoString(id);
	return nullptr;
}
