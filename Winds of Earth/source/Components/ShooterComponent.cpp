#include "ShooterComponent.h"
#include "TeamComponent.h"
#include "ProjectileComponent.h"
#include "ComponentSystem/Image.h"
#include "../../WOE_Engine/source/ComponentSystem/Collision.h"
#include "SelectorComponent.h"
#include "ClickMovement.h"
#include "SightMovement.h"

void ShooterComponent::Update([[maybe_unused]] double deltaTime)
{
	int team = 0;
	// Get our team. 
	auto teamComponent = m_pOwner->GetComponent<TeamComponent>("TeamComponent");
	if (teamComponent)
		team = teamComponent->GetTeamNumber();

	if (m_targetID != 0)
	{
		const auto& targetTeam = m_pTarget->GetComponent<TeamComponent>("TeamComponent");
		if (targetTeam) 
		{
			if (targetTeam->GetTeamNumber() == team)
				return;

			const auto& targetLocation = m_pTarget->GetLocation();
			// now we need to find the distance to the enemy.
			auto targetDistance = sqrtf(std::powf((targetLocation.x - m_pOwner->GetLocation().x), 2) + std::powf((targetLocation.y - m_pOwner->GetLocation().y), 2));
			if (m_range > targetDistance)
			{
				const auto& click = m_pOwner->GetComponent<ClickMovement>("ClickMovement");
				if (click)
					click->StopMovement();
				const auto& sight = m_pOwner->GetComponent<SightMovement>("SightComponent");
				if (sight)
					sight->StopMovement();
			}
		}
	}
	if (m_delay > 0)
	{
		m_delay -= deltaTime;
	}
	else
	{
		// Here if we have a target we will shoot it instead. 
		if (m_targetID != 0)
		{
			const auto& targetTeam = m_pTarget->GetComponent<TeamComponent>("TeamComponent");
			if (targetTeam)
			{
				if (targetTeam->GetTeamNumber() == team)
					return;

				const auto& targetLocation = m_pTarget->GetLocation();
				// now we need to find the distance to the enemy.
				auto targetDistance = sqrtf(std::powf((targetLocation.x - m_pOwner->GetLocation().x), 2) + std::powf((targetLocation.y - m_pOwner->GetLocation().y), 2));
				if (m_range > targetDistance)
				{
					const auto& click = m_pOwner->GetComponent<ClickMovement>("ClickMovement");
					if (click)
						click->StopMovement();
					const auto& sight = m_pOwner->GetComponent<SightMovement>("SightComponent");
					if (sight)
						sight->StopMovement();

					// Spawn a bullet. 
					Entity* pBullet = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager()->CreateEntity(m_round);
					// Set Bullet Speed and direction. while also using pawns spawning location.  
					pBullet->SetLocation(Vec2(m_pOwner->GetLocation().x, m_pOwner->GetLocation().y));
					auto proj = pBullet->GetComponent<ProjectileComponent>("ProjectileComponent");
					proj->SetSpeed(m_projectileSpeed);
					// figure out direction and normalize. 
					Vec2 direction = { (targetLocation.x - m_pOwner->GetLocation().x), (targetLocation.y - m_pOwner->GetLocation().y) };
					direction.Normalize();
					proj->SetDirection(direction);

					// Set Team for the bullet. 
					auto bulletTeam = pBullet->GetComponent<TeamComponent>("TeamComponent");
					bulletTeam->SetTeamNumber(team);
					// Reset the firing delay.
					m_delay = m_fireRate;
					return;
				}
			}
		}

		if (team != 0)
		{
			const auto pEntityManager = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager();
			for (const auto pEntity : pEntityManager->GetAllEntities())
			{
				// We skip ourselves.
				if (m_pOwner == pEntity)
					continue;
				int teamInQuestion = 0;

				auto enemyTeam = pEntity->GetComponent<TeamComponent>("TeamComponent");
				if (enemyTeam)
					teamInQuestion = enemyTeam->GetTeamNumber();

				// only check enemy team members. 
				if (team != teamInQuestion && teamInQuestion != 0)
				{
					auto isProjectile = pEntity->GetComponent<ProjectileComponent>("ProjectileComponent");
					if (isProjectile)
						continue;

					const auto& enemyLocation = pEntity->GetLocation();
					// now we need to find the distance to the enemy.
					auto tempDistance = sqrtf(std::powf((enemyLocation.x - m_pOwner->GetLocation().x), 2) + std::powf((enemyLocation.y - m_pOwner->GetLocation().y), 2));
					if (m_range > tempDistance)
					{

						// Spawn a bullet. 
						Entity* pBullet = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager()->CreateEntity(m_round);
						// Set Bullet Speed and direction. while also using pawns spawning location.  
						pBullet->SetLocation(Vec2(m_pOwner->GetLocation().x, m_pOwner->GetLocation().y));
						auto proj = pBullet->GetComponent<ProjectileComponent>("ProjectileComponent");
						proj->SetSpeed(m_projectileSpeed);
						// figure out direction and normalize. 
						Vec2 direction = { (enemyLocation.x - m_pOwner->GetLocation().x), (enemyLocation.y - m_pOwner->GetLocation().y) };
						direction.Normalize();
						proj->SetDirection(direction);

						// Set Team for the bullet. 
						auto bulletTeam = pBullet->GetComponent<TeamComponent>("TeamComponent");
						bulletTeam->SetTeamNumber(team);
						// Reset the firing delay.
						m_delay = m_fireRate;
						return;
					}
				}
			}
		}
	}
}

bool ShooterComponent::ProcessEvent(Event& evt)
{
	if (evt.GetEventType() == Event::EventType::DestroyEntity)
	{
		if (evt.GetTargetEntityID() == m_targetID)
		{
			ClearTarget();
		}
	}

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
					if (m_pOwner == pEntity)
						continue;

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
				ClearTarget();
			}
			// Here we tried to move instead select maybe so we need to erase the target. 
		}
	}
	return false;
}
