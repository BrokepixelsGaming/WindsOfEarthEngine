#include "EntityManager.h"
#include "tools/BleachNew.h"
#include "ComponentSystem/RenderRect.h"
#include "EventSystem/EventManager.h"
#include "EngineInterface/EngineSingleton.h"
#include "EventSystem/EventManager.h"
#include "ComponentSystem/MoveComponent.h"
#include "ComponentSystem/PlayerMoveComponent.h"
#include "ComponentSystem/EnemyMoveComponent.h"
#include "ComponentSystem/CollectorComponent.h"
#include "ComponentSystem/Collision.h"
#include "../../Winds of Earth/source/Components/GameComponentFactory.h"
#include "ComponentSystem/Image.h"
#include "ComponentSystem/UITextField.h"



EntityManager::EntityManager()
	: m_entities()
	, m_uiEntities()
{
}

EntityManager::~EntityManager()
{
	for (auto& entity : m_entities)
	{
		BLEACH_DELETE(entity);
	}
	for (auto& entity : m_uiEntities)
	{
		BLEACH_DELETE(entity);
	}
}

void EntityManager::UpdateEntities(double deltaTime)
{
	//for (auto& entity : m_entities)
	//{
	//	entity->Update(deltaTime);
	//}
	// We check the size before we start our loop so that we don't invalidate a iterator. This is a problem with entities spawning during update. 
	// I may look at a way to handle a delay spawning to fix this. for now this works. 
	size_t size = m_entities.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_entities[i]->Update(deltaTime);
	}
}

void EntityManager::Render(SDL_Renderer* pRenderer)
{
	Vec2 cameraOffset = { static_cast<float>(EngineSingleton::Get()->GetCamera().GetOffsetX()), static_cast<float>(EngineSingleton::Get()->GetCamera().GetOffsetY()) };
	for (auto& entity : m_entities)
	{
		entity->Render(pRenderer, cameraOffset);
	}
	for (auto& uiEntity : m_uiEntities)
	{
		uiEntity->Render(pRenderer, Vec2{0,0});
	}
}

Entity* EntityManager::CreateEntity(std::string command)
{
	std::string dataStream = "data/EntityData/";
	dataStream.append(command);
	dataStream.append(".xml");

	tinyxml2::XMLDocument document;
	auto result = document.LoadFile(dataStream.c_str());
	if (result != tinyxml2::XML_SUCCESS)
	{
		std::cerr << document.ErrorStr() << "\n" << " Failed to get " << dataStream << "\n";
		return nullptr;
	}
	// get the root
	const tinyxml2::XMLElement* pRoot = document.RootElement();

	if (pRoot == nullptr)
	{
		std::cout << "Root is invalid of " << command << "\n";
		return nullptr;
	}
	else
	{
		return CreateEntity(pRoot);
	}
}

Entity* EntityManager::CreateEntity(const tinyxml2::XMLElement* pEntity)
{
	float x;
	float y;
	const auto pTransform = pEntity->FirstChildElement("Transform");
	if (!pTransform)
	{
		return nullptr;
	}
	const auto pPosition = pTransform->FirstChildElement("position");
	if (pPosition->FindAttribute("x")->QueryFloatValue(&x) != tinyxml2::XML_SUCCESS)
	{
		return nullptr;
	}
	if (pPosition->FindAttribute("y")->QueryFloatValue(&y) != tinyxml2::XML_SUCCESS)
	{
		return nullptr;
	}

	const auto pUIType = pEntity->FirstChildElement("EntityTypeUI");
	if (pUIType)
	{
		// Entity boiler plate exists at this point.
		m_uiEntities.push_back(BLEACH_NEW(Entity(Vec2{ x, y })));
		auto pComponents = pEntity->FirstChildElement("Components");

		LoadEntityComponents(pComponents, m_uiEntities.back());

		EventManager* pEventMangager = EngineSingleton::Get()->GetEventManager();
		pEventMangager->AddListener(m_uiEntities.back());
		return m_uiEntities.back();
	}
	else 
	{
		// Entity boiler plate exists at this point.
		m_entities.push_back(BLEACH_NEW(Entity(Vec2{ x, y })));
		auto pComponents = pEntity->FirstChildElement("Components");

		LoadEntityComponents(pComponents, m_entities.back());

		EventManager* pEventMangager = EngineSingleton::Get()->GetEventManager();
		pEventMangager->AddListener(m_entities.back());
		return m_entities.back();
	}
}

void EntityManager::CreateEntityPair([[maybe_unused]]const tinyxml2::XMLElement* pElementOfPair)
{
	//const tinyxml2::XMLElement* pEntity = pElementOfPair->FirstChildElement();
	//Entity* pEntityA = CreateEntity(pEntity);
	//pEntity = pEntity->NextSiblingElement();
	//Entity* pEntityB = CreateEntity(pEntity);
	// TODO:: once the team creation components are loaded
	//auto componentA = pEntityA->GetComponent<TeamControllerComponent>("TeamController"); //Will need to do something to the effect of getting
	//auto componentB = pEntityB->GetComponent<TeamControllerComponent>("TeamController");																					 //	both components and linking them together.Each only certain unit's or buildings will need to have a 
																						 //	point to their team captain 
	//componentA->LinkEnemyController(pEntityB);
	//componentB->LinkEnemyController(pEntityA);
}

void EntityManager::LoadEntityComponents(const tinyxml2::XMLElement* pComponents, Entity* pEntity)
{
	///////////////////////////////////////////////////////////////
	// now to add components to the entity
	///////////////////////////////////////////////////////////////// 
	for (const tinyxml2::XMLElement* pComponent = pComponents->FirstChildElement(); pComponent != nullptr; pComponent = pComponent->NextSiblingElement())
	{
		const char* componentName = pComponent->Name();

		auto* returnedComponent = EngineSingleton::Get()->GetGameComponentFactory()->ParseComponent(pComponent, pEntity);
		if (returnedComponent != nullptr)
		{
			pEntity->AddComponent(returnedComponent);
			continue;
		}
		if (std::strcmp(componentName, "EnemyMoveComponent") == 0)
		{
			pEntity->AddComponent(BLEACH_NEW(EnemyMoveComponent(pEntity)));
			continue;
		}
		if (std::strcmp(componentName, "CollectorComponent") == 0)
		{
			pEntity->AddComponent(BLEACH_NEW(CollectorComponent(pEntity)));
			continue;
		}
		if (std::strcmp(componentName, "Image") == 0)
		{
			const char* imagePath;
			int height = 0;
			int width = 0;

			pComponent->FirstChildElement("height")->FirstAttribute()->QueryIntValue(&height);
			pComponent->FirstChildElement("width")->FirstAttribute()->QueryIntValue(&width);
			imagePath = pComponent->FirstChildElement("imagepath")->GetText();
			pEntity->AddComponent(BLEACH_NEW(Image(pEntity, imagePath, EngineSingleton::Get()->GetWindow()->GetRenderer(), static_cast<float>(width), static_cast<float>(height) )));
			continue;
		}
		if (std::strcmp(componentName, "Collider") == 0)
		{
			int height = 0;
			int width = 0;

			pComponent->FirstChildElement("height")->FirstAttribute()->QueryIntValue(&height);
			pComponent->FirstChildElement("width")->FirstAttribute()->QueryIntValue(&width);
			const char* message = pComponent->FirstChildElement("message")->GetText();
			pEntity->AddComponent(BLEACH_NEW(Collision(pEntity, static_cast<float>(height), static_cast<float>(width), EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetCollisionRef(), message)));
			continue;
		}
		if (std::strcmp(componentName, "RenderRect") == 0)
		{
			int height = 0;
			int width = 0;
			int green = 0;
			int blue = 0;
			int red = 0;
			int alpha = 0;

			pComponent->FirstChildElement("height")->FirstAttribute()->QueryIntValue(&height);
			pComponent->FirstChildElement("width")->FirstAttribute()->QueryIntValue(&width);
			// load color.
			auto pColor = pComponent->FirstChildElement("Color");
			pColor->FirstChildElement("g")->FirstAttribute()->QueryIntValue(&green);
			pColor->FirstChildElement("b")->FirstAttribute()->QueryIntValue(&blue);
			pColor->FirstChildElement("r")->FirstAttribute()->QueryIntValue(&red);
			pColor->FirstChildElement("a")->FirstAttribute()->QueryIntValue(&alpha);

			pEntity->AddComponent(BLEACH_NEW(RenderRect(Rect{ height, width }, Vec2{ pEntity->GetLocation().x, pEntity->GetLocation().y }, Color{ static_cast<unsigned char>(green),static_cast<unsigned char>(blue),static_cast<unsigned char>(red),static_cast<unsigned char>(alpha) }, pEntity))); // this is a temp
		}
		// Add in when n
		//if (std::strcmp(componentName, "UITextField") == 0)
		//{
		//	TTF_Font* pFont;
		//	const char* text = "";
		//	float x = 0;
		//	float y = 0;
		//	float h = 0;
		//	float w = 0;
		//	int r = 0;
		//	int b = 0;
		//	int g = 0;
		//	int A = 0;
		//	text = pComponent->FirstChildElement("Text")->FindAttribute("Text")->Value();
		//	pComponent->FirstChildElement("Location")->FindAttribute("x")->QueryFloatValue(&x);
		//	pComponent->FirstChildElement("Location")->FindAttribute("y")->QueryFloatValue(&y);
		//	pComponent->FirstChildElement("Location")->FindAttribute("h")->QueryFloatValue(&h);
		//	pComponent->FirstChildElement("Location")->FindAttribute("w")->QueryFloatValue(&w);
		//	pComponent->FirstChildElement("Color")->FindAttribute("r")->QueryIntValue(&r);
		//	pComponent->FirstChildElement("Color")->FindAttribute("b")->QueryIntValue(&b);
		//	pComponent->FirstChildElement("Color")->FindAttribute("g")->QueryIntValue(&g);
		//	pComponent->FirstChildElement("Color")->FindAttribute("A")->QueryIntValue(&A);
		//	auto color = SDL_Color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(A) };
		//	pEntity->AddComponent(BLEACH_NEW(UITextField(pFont, text, color, SDL_FRect{ x,y,w,h }, EngineSingleton::Get()->GetWindow()->GetRenderer())));
		//}
	}
}

///////////////////////////////////////////////////////////////
// Should only be called during the initalization of a new world. 
// As this will clear any previous entities that may exisit for a 
//	clean slate.  
///////////////////////////////////////////////////////////////
bool EntityManager::LoadEntities(const tinyxml2::XMLElement* pEntites)
{
	// Create Entity here. This is where we would loop thru file data. 
	m_entities.clear();// ensure we are working with a clean slate. 
	for (const tinyxml2::XMLElement* pEntity = pEntites->FirstChildElement(); pEntity != nullptr; pEntity = pEntity->NextSiblingElement())
	{
		if (std::strcmp(pEntity->Name(), "PairedEntities") == 0)
		{
			CreateEntityPair(pEntity);
		}
		else 
		{
			CreateEntity(pEntity);
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////
// This is terrible and it's the cost of storing the entities in a vector.
//	This is to ensure the correct entity is destroyed and removed. 
//	May need an update at some point but will only change if comes up,
//	in a profile. 
///////////////////////////////////////////////////////////////
void EntityManager::DestroyEntity(EntityID id)
{
	int index = 0;
	for (auto& entity : m_entities)
	{
		if (entity->GetID() == id)
		{
			if (entity != nullptr)
			{
				BLEACH_DELETE(entity);
				break;
			}
		}
		++index;
	}
	int ii = 0;
	for (auto i = m_entities.begin(); i < m_entities.end(); ++i)
	{


		if (ii == index)
		{
			m_entities.erase(i);
			break;
		}
		++ii;
	}
}
