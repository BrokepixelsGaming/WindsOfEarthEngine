#pragma once
#include "ComponentSystem\Component.h"
#include "EntitySystem/Entity.h"

class TeamComponent :
    public Component
{
protected:
	StringId m_type = "TeamComponent";
	int m_teamNumber;
public:
	TeamComponent(int teamNumber)
		: m_teamNumber(teamNumber)
	{}
	int GetTeamNumber()const { return m_teamNumber; };
	void SetTeamNumber(int teamNumber) { m_teamNumber = teamNumber; }
	StringId GetType() override { return m_type; }
	

};

