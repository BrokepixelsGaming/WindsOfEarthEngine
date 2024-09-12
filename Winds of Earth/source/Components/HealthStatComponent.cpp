#include "HealthStatComponent.h"
#include "ComponentSystem/Image.h"
#include "EngineInterface/EngineSingleton.h"
#include "TeamComponent.h"

HealthStatComponent::HealthStatComponent(Entity* pOwner, SDL_Renderer* pRenderer, SDL_FRect transform, int heartValue, int numHearts, int maxHealth)
	: m_animation("data/Textures/Warzone/Hearts.png", 1, 20, 17, pRenderer, 0, 0)
	, m_pOwner(pOwner)
	, m_transform(transform)
	, m_currentHealth(maxHealth)
	, m_heartValue(heartValue)
	, m_numHearts(numHearts)
	, m_maxHealth(maxHealth)
{
	// Adds all the animations for the player character. 
	m_animation.AddAnimationSequence("Full", 0, 0);
	m_animation.AddAnimationSequence("Half", 1, 1);
	m_animation.AddAnimationSequence("Empty", 2, 2);
	//m_animation.PlayAnimation("Full");
}

HealthStatComponent::HealthStatComponent(Entity* pOwner, SDL_Renderer* pRenderer, SDL_FRect transform, int heartValue, int numHearts, int maxHealth, const char* healType)
	: m_animation("data/Textures/Warzone/Hearts.png", 1, 20, 17, pRenderer, 0, 0)
	, m_pOwner(pOwner)
	, m_transform(transform)
	, m_currentHealth(maxHealth)
	, m_heartValue(heartValue)
	, m_numHearts(numHearts)
	, m_maxHealth(maxHealth)
	, m_healType(healType)
{
	// Adds all the animations for the player character. 
	m_animation.AddAnimationSequence("Full", 0, 0);
	m_animation.AddAnimationSequence("Half", 1, 1);
	m_animation.AddAnimationSequence("Empty", 2, 2);
	//m_animation.PlayAnimation("Full");
}

void HealthStatComponent::Render(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
	if (m_currentHealth <= 0)
	{
		//destroy self. 
		EngineSingleton::Get()->GetEventManager()->QueueEvent(Event(Event::EventType::DestroyEntity, m_pOwner->GetID(), "Self Destroy"));
		return;
	}

	double fakeHealth = m_currentHealth;

	// Resetting the Transform back to the starting point. 
	const auto& unitImage = m_pOwner->GetComponent<Image>("Image");
	int hearts = static_cast<int>(m_maxHealth) / m_heartValue;
	m_transform.x -= cameraOffset.x + (unitImage->m_transform.w / 2);
	m_transform.y -= cameraOffset.y + ((unitImage->m_transform.h) - m_transform.h);


	for (int i = 0; i < hearts; ++i) // how many hearts to show. 
	{
		if (fakeHealth >= ((i + 1) * m_heartValue))
		{
			m_animation.PlayAnimation("Full");
		}
		else if (fakeHealth >= (i * m_heartValue) + 1 )
		{
			m_animation.PlayAnimation("Half");
		}
		else
		{
			m_animation.PlayAnimation("Empty");
		}

		m_animation.Render(pRenderer, m_transform, cameraOffset); // render 1 heart.
		m_transform.x += m_transform.w; // move the heart to the right. 
	}

	m_transform.x = m_pOwner->GetLocation().x;
	m_transform.y = m_pOwner->GetLocation().y;
	
}

bool HealthStatComponent::ProcessEvent(Event& evt)
{
	if (evt.GetTargetEntityID() == m_pOwner->GetID())
	{
		if (evt.GetEventType() == Event::EventType::Collision)
		{
			//if(evt.GetCommandString() != m_pOwner->GetComponent<Collision>("Collision")->GetMessage())
			if(evt.GetCommandString() != "red" && evt.GetCommandString() != "green")
			{
				if (evt.GetCommandString() == "Bullet")
					m_currentHealth -= 1;
				if (evt.GetCommandString() == "HeroTankShell")
					m_currentHealth -= 6;
				if (evt.GetCommandString() == "TankShell")
					m_currentHealth -= 4;
			}

		}
	}
	return false;
}
