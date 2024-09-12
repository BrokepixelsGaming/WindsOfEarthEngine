#include "MeleeComponent.h"
#include "HealthStatComponent.h"
#include "ClickMovement.h"
#include "SightMovement.h"

void MeleeComponent::Update(double deltaTime)
{
	// Check if we are in range of our target and if we are we will stop all movement. 
	if (m_targetID != 0)
	{
		const auto& targetLocation = m_pTarget->GetLocation();
		// now we need to find the distance to the enemy.
		auto targetDistance = sqrtf(std::powf((targetLocation.x - m_pOwner->GetLocation().x), 2) + std::powf((targetLocation.y - m_pOwner->GetLocation().y), 2));
		if (m_range > targetDistance)
		{
			const auto& click = m_pOwner->GetComponent<ClickMovement>("ClickMovement");
			if (click)
				click->StopMovement();
			const auto& sight = m_pOwner->GetComponent<SightMovement>("SightMovement");
			if (sight)
				sight->StopMovement();
		}
	}
	if (m_delay > 0)
	{
		m_delay -= deltaTime;
	}
	else
	{
		int team = 0;
		// Get our team. 
		auto teamComponent = m_pOwner->GetComponent<TeamComponent>("TeamComponent");
		if (teamComponent)
			team = teamComponent->GetTeamNumber();

		// Here if we have a target we will shoot it instead. 
		if (m_targetID != 0)
		{
			const auto& targetLocation = m_pTarget->GetLocation();
			// now we need to find the distance to the enemy.
			auto targetDistance = sqrtf(std::powf((targetLocation.x - m_pOwner->GetLocation().x), 2) + std::powf((targetLocation.y - m_pOwner->GetLocation().y), 2));
			const auto& targetTeam = m_pTarget->GetComponent<TeamComponent>("TeamComponent");
			if (targetTeam) 
			{
				if (targetTeam->GetTeamNumber() == team)
				{
					// if we are a healer we need to heal but if not we need to do nothing.
					if (m_healer)
					{

						if (m_range > targetDistance)
						{
							const auto& healthStat = m_pTarget->GetComponent<HealthStatComponent>("HealthStatComponent");

							if (m_typeToHeal == healthStat->GetHealType())
							{
								m_pTarget->GetComponent<HealthStatComponent>("HealthStatComponent")->UpdateHealth(m_damage);
								m_delay = m_fireRate;
								return;
							}
							else
							{
								return;
							}
						}
					}
				}
				// if we are opposing teams we will do damage. 
				if (m_range > targetDistance)
				{
					const auto& click = m_pOwner->GetComponent<ClickMovement>("ClickMovement");
					if (click)
						click->StopMovement();
					const auto& sight = m_pOwner->GetComponent<SightMovement>("SightComponent");
					if (sight)
						sight->StopMovement();
					m_pTarget->GetComponent<HealthStatComponent>("HealthStatComponent")->UpdateHealth(-m_damage);
					m_delay = m_fireRate;
					return;
				}
			}
		}
	}
}

bool MeleeComponent::ProcessEvent(Event& evt)
{
	if (evt.GetEventType() == Event::EventType::DestroyEntity)
	{
		if (evt.GetTargetEntityID() == m_targetID)
		{
			ClearTarget();
		}
	}

	// Check if we are Selected
	const auto& selector = m_pOwner->GetComponent<SelectorComponent>("SelectorComponent");
	// Check if we are Selected
	if (selector == nullptr)
		return false;

	if (m_pOwner->GetComponent<SelectorComponent>("SelectorComponent")->IsSelected())
	{
		if (evt.GetSDLEvent()->type == SDL_MOUSEBUTTONDOWN)
		{
			if (evt.GetSDLEvent()->button.button == '\x3')
			{
				// if we are selected We need to figure out who is at where we have indicated. 
				const auto pEntityManager = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager();
				for (const auto pEntity : pEntityManager->GetAllEntities())
				{

					auto hitBox = pEntity->GetComponent<Collision>("Collision");
					// perform a hit check. 
					if (hitBox)
					{
						if (hitBox->HitCheck(static_cast<float>(evt.GetSDLEvent()->button.x + EngineSingleton::Get()->GetCamera().GetOffsetX()), static_cast<float>(evt.GetSDLEvent()->button.y + (EngineSingleton::Get()->GetCamera().GetOffsetY()))))
						{
							// Set as target. 
							m_pTarget = pEntity;
							m_targetID = pEntity->GetID();
							return true;
						}
					}
				}
				// Here we tried to move instead select maybe so we need to erase the target. 
				ClearTarget();
			}
		}
	}
	return false;
}
