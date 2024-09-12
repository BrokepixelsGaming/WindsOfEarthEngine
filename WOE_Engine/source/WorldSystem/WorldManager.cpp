#include "WorldManager.h"

#include <assert.h>

#include "EngineInterface/EngineSingleton.h"
#include "tools\Utilities\StringId.h"	
#include "tools/Utilities/tinyxml2.h"


void WorldManager::UpdateWorlds(double deltatime)
{
	for (size_t i = 0; i < kNumberOfWorlds; ++i)
	{
		if (m_worlds[i].IsActive())
		{
			m_worlds[i].UpdateWorld(deltatime);
		}
	}
}

///////////////////////////////////////////////////////////////
// // TODO(Shilo): Need to figure how I want to load the data for each world. Maybe a vector of strings that I store and loop thru. As a temporary solution. 
// but for now I am hardcoding for only 1 world. this is where I would need to have some external document with the list of each worlds data and where to find it. 
///////////////////////////////////////////////////////////////
bool WorldManager::LoadWorldsData()
{
	const char* mapFileString = "";
	const char* musicFileString = "";
	tinyxml2::XMLDocument document;
	auto result = document.LoadFile(kWorldInitDataLocation);
	if (result != tinyxml2::XML_SUCCESS)
	{
		//std::cerr << document.ErrorStr() << "\n" << "Failed to get WorldData\n";
		return false;
	}
	// get the root
	const tinyxml2::XMLElement* pRoot = document.RootElement();

	assert(pRoot);

	const tinyxml2::XMLElement* pMap = pRoot->FirstChildElement("Map");
	if (pMap)
	{
		mapFileString = pMap->FirstChildElement("MapData")->GetText();
		// here I need to get the WorldsDataFile
		
	}
	
	m_worlds[0].SetMap(static_cast<TileMap*>(EngineSingleton::Get()->GetResourceManager()->Cache(StringId(mapFileString))));
	// Set audio for the world. 
	const tinyxml2::XMLElement* pAudio = pRoot->FirstChildElement("Audio");
	if (pAudio)
	{
		musicFileString = pAudio->FirstChildElement("Music")->GetText();
	}
	// here I need to get the WorldsDataFile
	m_worlds[0].SetMusic(static_cast<Audio*>(EngineSingleton::Get()->GetResourceManager()->Cache(StringId(musicFileString))));
	SetCurrentWorld(0);
	const tinyxml2::XMLElement* pEntities = pRoot->FirstChildElement("Entities");
	if(pEntities)
	{
		m_worlds[0].LoadEntityData(pEntities);
	}
	
	return true;
}

///////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////
void WorldManager::DisplayActiveWorld(SDL_Renderer* pRenderer)
{
	for (auto& world : m_worlds)
	{
		if (world.IsActive())
		{
			SDL_RenderClear(pRenderer);
			world.Render(pRenderer);
			if (kAudioEnabled)
			{
				world.PlayMusic();
			}
		}
	}
}

void WorldManager::SetCurrentWorld(int index)
{
	m_worlds[index].SetActive();
	m_currentWorld = &m_worlds[index];
}
