#include "GameComponentFactory.h"
#include "tools/BleachNew.h"
#include "ComponentSystem/PlayerMoveComponent.h"
#include "ComponentSystem/Animation.h"
#include "InteractableComponent.h"
#include "BuildComponent.h"
#include "SpawnerComponent.h"
#include "StatComponent.h"
#include "ButtonComponent.h"
#include "ComponentSystem/Image.h"
#include "StatBarComponent.h"
#include "EngineInterface/EngineSingleton.h"
#include "SDL/SDL.h"
#include "SelectorComponent.h"
#include "ClickMovement.h"
#include "TeamComponent.h"
#include "ShooterComponent.h"
#include "ProjectileComponent.h"
#include "HealthStatComponent.h"
#include "MeleeComponent.h"
#include "SightMovement.h"
#include "DiggerComponent.h"
#include "../Components/ScoreBoardComponent.h"
#include "MapGeneratorComponent.h"
#include "CellularAutomaton.h"
#include "LSystemComponent.h"
#include "WorldGenerationManager.h"

Component* GameComponentFactory::ParseComponent(const tinyxml2::XMLElement* pComponent, Entity* pOwner)
{
	const char* componentName = pComponent->Name();

	//EngineSingleton::Get()->GetEventManager()->QueueEvent()
	// Example of how to add a component from the game portion. 
	if (std::strcmp(componentName, "PlayerMoveComponent") == 0)
	{
		return BLEACH_NEW(PlayerMoveComponent(pOwner));
	}
	if (std::strcmp(componentName, "DiggerComponent") == 0)
	{
		return BLEACH_NEW(DiggerComponent(pOwner));
	}
	if (std::strcmp(componentName, "WorldGenerationManager") == 0)
	{
		return BLEACH_NEW(WorldGenerationManager(pOwner));
	}
	if (std::strcmp(componentName, "CellularAutomaton") == 0)
	{
		return BLEACH_NEW(CellularAutomaton(pOwner));
	}
	if (std::strcmp(componentName, "LSystemComponent") == 0)
	{
		return BLEACH_NEW(LSystemComponent(pOwner));
	}
	if (std::strcmp(componentName, "MapGeneratorComponent") == 0)
	{
		return BLEACH_NEW(MapGeneratorComponent(pOwner));
	}
	if (std::strcmp(componentName, "ScoreBoard") == 0)
	{
		float x;
		float y;

		const auto pPosition = pComponent->FirstChildElement("position");
		if (pPosition->FindAttribute("x")->QueryFloatValue(&x) != tinyxml2::XML_SUCCESS)
		{
			return nullptr;
		}
		if (pPosition->FindAttribute("y")->QueryFloatValue(&y) != tinyxml2::XML_SUCCESS)
		{
			return nullptr;
		}
		return BLEACH_NEW(ScoreBoardComponent(pOwner, SDL_FRect(x,y)));
	}
	
	if (std::strcmp(componentName, "InteractableComponent") == 0)
	{
		float height = 0;
		float width = 0;

		pComponent->FirstChildElement("height")->FirstAttribute()->QueryFloatValue(&height);
		pComponent->FirstChildElement("width")->FirstAttribute()->QueryFloatValue(&width);

		return BLEACH_NEW(InteractableComponent(pOwner, Vec2(width,height)));
	}
	if (std::strcmp(componentName, "HealthStatComponent") == 0)
	{
		float height = 0;
		float width = 0;
		int heartValue = 0;
		int numHearts = 0;
		int maxHealth = 0;

		pComponent->FirstChildElement("height")->FirstAttribute()->QueryFloatValue(&height);
		pComponent->FirstChildElement("width")->FirstAttribute()->QueryFloatValue(&width);
		pComponent->FirstChildElement("heartValue")->FirstAttribute()->QueryIntValue(&heartValue);
		pComponent->FirstChildElement("numHearts")->FirstAttribute()->QueryIntValue(&numHearts);
		pComponent->FirstChildElement("maxHealth")->FirstAttribute()->QueryIntValue(&maxHealth);
		auto healType = pComponent->FirstChildElement("healType");
		if (healType)
		{
			const char* type = healType->FirstAttribute()->Name();
			return BLEACH_NEW(HealthStatComponent(pOwner, EngineSingleton::Get()->GetWindow()->GetRenderer(), SDL_FRect(pOwner->GetLocation().x, pOwner->GetLocation().y, height, width), heartValue, numHearts, maxHealth, type ));
		}
		return BLEACH_NEW(HealthStatComponent(pOwner, EngineSingleton::Get()->GetWindow()->GetRenderer(), SDL_FRect(pOwner->GetLocation().x, pOwner->GetLocation().y, height, width), heartValue, numHearts, maxHealth));
	}

	if (std::strcmp(componentName, "ButtonComponent") == 0)
	{
		float height = 0;
		float width = 0;

		pComponent->FirstChildElement("height")->FirstAttribute()->QueryFloatValue(&height);
		pComponent->FirstChildElement("width")->FirstAttribute()->QueryFloatValue(&width);

		return BLEACH_NEW(ButtonComponent(pOwner, Vec2(width, height)));
	}

	if (std::strcmp(componentName, "BuildComponent") == 0)
	{
		return BLEACH_NEW(BuildComponent(pOwner));
	}
	if (std::strcmp(componentName, "ClickMovement") == 0)
	{
		auto flyer = pComponent->FirstChildElement("flyer");
		if (flyer)
		{
			return BLEACH_NEW(ClickMovement(pOwner, true));
		}
		return BLEACH_NEW(ClickMovement(pOwner));
	}
	if (std::strcmp(componentName, "SightMovement") == 0)
	{
		int sightRange = 0;
		pComponent->FirstChildElement("sightRange")->FirstAttribute()->QueryIntValue(&sightRange);

		auto flyer = pComponent->FirstChildElement("flyer");
		if (flyer)
		{
			return BLEACH_NEW(SightMovement(pOwner, sightRange, true));
		}
		return BLEACH_NEW(SightMovement(pOwner, sightRange));
	}
	if (std::strcmp(componentName, "ProjectileComponent") == 0)
	{
		return BLEACH_NEW(ProjectileComponent(pOwner));
	}
	if (std::strcmp(componentName, "ShooterComponent") == 0)
	{
		int range = 0;
		int damage = 0;
		double fireRate = 0;
		float projectileSpeed = 0;


		pComponent->FirstChildElement("range")->FirstAttribute()->QueryIntValue(&range);
		pComponent->FirstChildElement("damage")->FirstAttribute()->QueryIntValue(&damage);
		pComponent->FirstChildElement("fireRate")->FirstAttribute()->QueryDoubleValue(&fireRate);
		pComponent->FirstChildElement("projectileSpeed")->FirstAttribute()->QueryFloatValue(&projectileSpeed);
		auto round = pComponent->FirstChildElement("round");
		if(round)
		{
			return BLEACH_NEW(ShooterComponent(pOwner, range, damage, fireRate, projectileSpeed, round->GetText()));
		}
		return BLEACH_NEW(ShooterComponent(pOwner, range, damage, fireRate, projectileSpeed));
	}
	if (std::strcmp(componentName, "MeleeComponent") == 0)
	{
		int range = 0;
		int damage = 0;
		double fireRate = 0;

		pComponent->FirstChildElement("range")->FirstAttribute()->QueryIntValue(&range);
		pComponent->FirstChildElement("damage")->FirstAttribute()->QueryIntValue(&damage);
		pComponent->FirstChildElement("fireRate")->FirstAttribute()->QueryDoubleValue(&fireRate);
		auto healer = pComponent->FirstChildElement("healer");
		if (healer)
		{
			const char* type = healer->FirstChildElement()->FirstAttribute()->Name();
			return BLEACH_NEW(MeleeComponent(pOwner, range, damage, fireRate, true, type));
		}
		return BLEACH_NEW(MeleeComponent(pOwner, range, damage, fireRate));
	}
	if (std::strcmp(componentName, "TeamComponent") == 0)
	{
		int teamNum = 0;
		pComponent->FirstChildElement("teamNum")->FirstAttribute()->QueryIntValue(&teamNum);
		return BLEACH_NEW(TeamComponent(teamNum));
	}
	if (std::strcmp(componentName, "SelectorComponent") == 0)
	{
		float height = 0;
		float width = 0;
		pComponent->FirstChildElement("height")->FirstAttribute()->QueryFloatValue(&height);
		pComponent->FirstChildElement("width")->FirstAttribute()->QueryFloatValue(&width);

		return BLEACH_NEW(SelectorComponent(pOwner, height, width));
	}

	if (std::strcmp(componentName, "SpawnerComponent") == 0)
	{
		const char* someString;

		someString = pComponent->FirstChildElement("entity")->GetText();

		return BLEACH_NEW(SpawnerComponent(someString));
	}

	if (std::strcmp(componentName, "StatComponent") == 0)
	{
		int currentHealth = 0;
		int maxHealth = 0;

		pComponent->FirstChildElement("currentHealth")->FirstAttribute()->QueryIntValue(&currentHealth);
		pComponent->FirstChildElement("maxHealth")->FirstAttribute()->QueryIntValue(&maxHealth);

		return BLEACH_NEW(StatComponent(pOwner, currentHealth, maxHealth));
	}
	if (std::strcmp(componentName, "StatBar") == 0)
	{
		const char* imagePath1;
		float height1 = 0;
		float width1 = 0;
		float xOfsset1 = 0;
		float yOfsset1 = 0;

		const char* imagePath2;
		float height2 = 0;
		float width2 = 0;
		float xOfsset2 = 0;
		float yOfsset2 = 0;

		pComponent->FirstChildElement("backHeight")->FirstAttribute()->QueryFloatValue(&height1);
		pComponent->FirstChildElement("backWidth")->FirstAttribute()->QueryFloatValue(&width1);
		pComponent->FirstChildElement("yOffset1")->FirstAttribute()->QueryFloatValue(&yOfsset1);
		imagePath1 = pComponent->FirstChildElement("backImagepath")->GetText();

		pComponent->FirstChildElement("frontHeight")->FirstAttribute()->QueryFloatValue(&height2);
		pComponent->FirstChildElement("frontWidth")->FirstAttribute()->QueryFloatValue(&width2);
		pComponent->FirstChildElement("yOffset2")->FirstAttribute()->QueryFloatValue(&yOfsset2);
		imagePath2 = pComponent->FirstChildElement("frontImagepath")->GetText();

		return BLEACH_NEW(StatBarComponent(pOwner, EngineSingleton::Get()->GetWindow()->GetRenderer(), imagePath1, imagePath2, width1, height1, width2, height2,Vec2(xOfsset1,yOfsset1), Vec2(xOfsset2, yOfsset2)));
	}
	//if (std::strcmp(componentName, "Animation") == 0)
	//{
		/*
		const char* spriteSheet = "data\\SpriteSheets\\GroceryIcon.png";
		double frameRate = 0;
		int frameWidth = 0;
		int frameHeight = 0;

		pComponent->FirstChildElement("frameRate")->FirstAttribute()->QueryDoubleValue(&frameRate);
		pComponent->FirstChildElement("frameWidth")->FirstAttribute()->QueryIntValue(&frameWidth);
		pComponent->FirstChildElement("frameHeight")->FirstAttribute()->QueryIntValue(&frameHeight);

		Animation* pAnimation = BLEACH_NEW(Animation(spriteSheet, frameRate, frameWidth, frameHeight, EngineSingleton::Get()->GetWindow()->GetRenderer(),0,0));

		pAnimation->AddAnimationSequence("Plane", 1, 1);
		pAnimation->AddAnimationSequence("Highligted", 2, 2);
		pAnimation->PlayAnimation("Plane");

		return pAnimation;
		*/
	//}
	return nullptr;
}
