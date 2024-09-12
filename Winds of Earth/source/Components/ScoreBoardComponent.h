#pragma once

#include "ComponentSystem/Component.h"
#include "ComponentSystem/UITextField.h"
#include "EntitySystem/Entity.h"
#include "EngineInterface/EngineSingleton.h"
#include "ComponentSystem/CollectorComponent.h"
#include <SDL/SDL_ttf.h>
#include <array>

class ScoreBoardComponent :
	public Component
{	
protected:
	Entity* m_pOwner;
	SDL_FRect m_origin;
	std::array<UITextField*, 7> m_textFields;
	
	UITextField* m_scoreField;
	TTF_Font* m_pFont;
	CollectorComponent* m_collector; //This is the component that collects and maintains score. 
	int m_currentScoreCount; // This is the current score that is being shown. 
	StringId m_type = "ScoreBoard";
public:
	ScoreBoardComponent(Entity* pOwner, SDL_FRect location);
	~ScoreBoardComponent();
	StringId GetType() override { return m_type; }
	virtual void Render([[maybe_unused]] SDL_Renderer* pRenderer, [[maybe_unused]] Vec2 cameraOffset) override;
	void Update([[maybe_unused]] double deltaTime) override;

};


