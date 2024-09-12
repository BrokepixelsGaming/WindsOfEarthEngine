#include "SightMovement.h"
#include "MeleeComponent.h"
#include "ShooterComponent.h"
#include "TeamComponent.h"
#include "tools/Utilities/Vector.h"

SightMovement::SightMovement(Entity* pOwner, int sightRange)
	: m_pOwner(pOwner)
	, m_sightRange(sightRange)
	, m_flyer(false)
	, m_targetLocation(pOwner->GetLocation())
	, m_pTarget(nullptr)
	, m_stopMoving(false)
{
}

SightMovement::SightMovement(Entity* pOwner, int sightRange, bool flyer)
	: m_pOwner(pOwner)
	, m_sightRange(sightRange)
	, m_flyer(flyer)
	, m_targetLocation(pOwner->GetLocation())
	, m_pTarget(nullptr)
	, m_stopMoving(false)
{
}

void SightMovement::Update(double deltaTime)
{
	bool hasTarget = false;
	// Check if this has any target either shooter or melee. 
	const auto& melee = m_pOwner->GetComponent<MeleeComponent>("MeleeComponent");
	if (melee)
		hasTarget = melee->HasTarget();
	const auto& shooter = m_pOwner->GetComponent<ShooterComponent>("ShooterComponent");
	if (shooter)
		hasTarget = shooter->HasTarget();
	
	if (!hasTarget) 
	{
		// need to see if anyone is in range. 
		int team = 0;
		// Get our team. 
		auto teamComponent = m_pOwner->GetComponent<TeamComponent>("TeamComponent");
		if (teamComponent)
			team = teamComponent->GetTeamNumber();

		const auto& pEntityManager = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager();
		int teamInQuestion = 0;
		for (const auto pEntity : pEntityManager->GetAllEntities())
		{
			// We skip ourselves.
			if (m_pOwner == pEntity)
				continue;

			const auto& enemyTeam = pEntity->GetComponent<TeamComponent>("TeamComponent");
			if (enemyTeam)
				teamInQuestion = enemyTeam->GetTeamNumber();

			auto isProjectile = pEntity->GetComponent<ProjectileComponent>("ProjectileComponent");
			if (isProjectile)
				continue;

			// only check enemy team members. 
			if (team != teamInQuestion && teamInQuestion != 0)
			{
				// Look to see if they are in our sight range. 
				const auto& targetLocation = pEntity->GetLocation();
				// now we need to find the distance to the enemy.
				auto targetDistance = sqrtf(std::powf((targetLocation.x - m_pOwner->GetLocation().x), 2) + std::powf((targetLocation.y - m_pOwner->GetLocation().y), 2));
				if (m_sightRange > targetDistance)
				{
					m_pTarget = pEntity;
					m_targetID = pEntity->GetID();
					m_targetLocation = targetLocation;
					if (melee)
						melee->SetTarget(pEntity);
					if (shooter)
						shooter->SetTarget(pEntity);

					break;
				}
			}
		}
	}
	else
	{
		// Check if Still a valid target. 
		//const auto& enemyTeam = m_pTarget->GetComponent<TeamComponent>("TeamComponent");
		if (m_targetID != 0)
		{
			int ownerRange = 0;
			if (melee)
				ownerRange = melee->GetRange();
			if (shooter)
				ownerRange = shooter->GetRange();
			// Look to see if they are in our sight range. 
			const auto& targetLocation = m_pTarget->GetLocation();
			// now we need to find the distance to the enemy.
			auto targetDistance = sqrtf(std::powf((targetLocation.x - m_pOwner->GetLocation().x), 2) + std::powf((targetLocation.y - m_pOwner->GetLocation().y), 2));
			if (m_sightRange > targetDistance)
			{
				// if we can't attack we need to move closer. 
				if (ownerRange < targetDistance)
					m_stopMoving = false;

				// if we can attack we shouldn't be moving. 
				if (!m_stopMoving)
					m_targetLocation = m_pTarget->GetLocation();
			}
		}
		else 
		{
			ClearTarget();
			m_stopMoving = false;
		}
	}

	// Walk towards our object. 
	StepMovement(deltaTime);
}

bool SightMovement::ProcessEvent(Event& evt)
{
	if (evt.GetEventType() == Event::EventType::DestroyEntity)
	{
		if (evt.GetTargetEntityID() == m_targetID)
		{
			ClearTarget();
			const auto& melee = m_pOwner->GetComponent<MeleeComponent>("MeleeComponent");
			if (melee)
				melee->ClearTarget();
			const auto& shooter = m_pOwner->GetComponent<ShooterComponent>("ShooterComponent");
			if (shooter)
				shooter->ClearTarget();
		}
	}
	return false;
}

void SightMovement::StopMovement()
{
	m_targetLocation = m_pOwner->GetLocation();
	m_stopMoving = true;
}

void SightMovement::StepMovement(double deltaTimeMs)
{
	// check to see if we've arrived
	Vec2 delta = m_targetLocation - GetPos();
	if (delta.Length() <= kPathingNodeTolerance)
	{
		return;
	}
	// step the game object towards their goal. we have not updated yet. 
	Vec2 direction = m_targetLocation - GetPos();
	direction.Normalize();
	Vec2 deltaPosition = direction * kDefaultSpeed * (float)deltaTimeMs;

	auto pos = GetPos();
	// check to see if we are a flyer and or ground and if we are outside our play area. 
	if (m_flyer)
	{
		bool clean = true;

		if (GetPos().x < kLowX)
		{
			m_pOwner->SetLocation(Vec2(kLowX, GetPos().y));
			clean = false;
		}
		if (GetPos().x > kMaxX)
		{
			m_pOwner->SetLocation(Vec2(kMaxX, GetPos().y));
			clean = false;
		}
		if (GetPos().y < kLowY)
		{
			m_pOwner->SetLocation(Vec2(GetPos().x, kLowY));
			clean = false;
		}
		if (GetPos().y > kMaxY)
		{
			m_pOwner->SetLocation(Vec2(GetPos().x, kMaxY));
			clean = false;
		}
		if (!clean)
			return;

	}
	else
	{
		bool clean = true;

		if (GetPos().x < kLowX)
		{
			m_pOwner->SetLocation(Vec2(kLowX, GetPos().y));
			clean = false;
		}
		if (GetPos().x > kMaxX)
		{
			m_pOwner->SetLocation(Vec2(kMaxX, GetPos().y));
			clean = false;
		}
		if (GetPos().y < kFenceYLimit)
		{
			m_pOwner->SetLocation(Vec2(GetPos().x, kFenceYLimit));
			clean = false;
		}
		if (GetPos().y > kMaxY)
		{
			m_pOwner->SetLocation(Vec2(GetPos().x, kMaxY));
			clean = false;
		}
		if (!clean)
			return;
	}

	m_pOwner->UpdateLocation(deltaPosition);
}
