#include "LSystemComponent.h"
#include "EngineInterface/EngineSingleton.h"

static constexpr float kStartingPositionOffsetForXAndY = 200.f;
static constexpr int kDragonRunTimes = 10;
static constexpr int kStarmapRunTimes = 12;
static constexpr float kDirectionStartingXValue = 0.0f;
static constexpr float kDirectionStartingYValue = -10.0f;
static constexpr double kStarburstTotalDegrees = 360;
static constexpr double kStarburstDegreeProgression = 15;
static constexpr int kNoiseSeedForStarColorRNG = 2134123;

static constexpr float kWhiteStarLimit = 0.50f;
static constexpr float kYellowStarLimit = 0.75f;
static constexpr float kOrangeStarLimit = 0.90f;


LSystemComponent::LSystemComponent(Entity* owner)
	: m_pOwner(owner)
{
	m_defaultPosition = { kStartingPositionOffsetForXAndY,  kStartingPositionOffsetForXAndY };
	m_defaultDirection = { kDirectionStartingXValue, kDirectionStartingYValue }; // we will point north. 
}

// I am using this as a means of control. 
void LSystemComponent::Update([[maybe_unused]]double deltaTimeMs)
{
	// Resets the Seed for Dragon Curve Rules. 
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kRKey))
		m_state = "F";

	// Run the Dragon Curve Rules 1 time. 
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kFKey))
		RunDragonCurveRules();

	// Runs the Dragon Curve Rules * kDragonRunTimes.
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kVKey))
		for (int i = 0; i < kDragonRunTimes; ++i)
			RunDragonCurveRules();
	
	// Reset the Seed for Starmap rule set.
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kTKey))
		m_state = "^R";

	// Run The Starmap Rules 1 time. 
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kGKey))
		RunStarmapRules();

	// Run the Starmap Rules * kStarmapRunTimes
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kBKey))
		for (int i = 0; i < kStarmapRunTimes; ++i)
			RunStarmapRules();

	// Resets the Camera back to the initial point. 
	if (EngineSingleton::Get()->GetKeyboard().WasKeyPressed(kYKey)) 
		EngineSingleton::Get()->GetCamera().ResetCamera();
}

// This will Render m_state. [???] is there a better way to do this loop? I feel like I puked continue; everywhere... The reason I made this choice was that only one action will be done, 
// at each letter. By using continue it will move onto the next Letter instead of comparing everything. Early outs.  
void LSystemComponent::Render(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
	m_position = m_defaultPosition;
	m_direction = m_defaultDirection;
	for (size_t i = 0; i < m_state.size(); i++)
	{
		//************************** Dragon Curve Render Logic***************************
		if (m_state[i] == 'F' || m_state[i] == 'G')
		{
			SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);
			DrawStraightLine(pRenderer, cameraOffset);
			continue;
		}
		if (m_state[i] == '+')
		{
			// I make a remark about not using a variable about this later. I think it hides what I am doing. Or it just ends up being an extra variable to say 90* 
			m_direction.RotateVectorByDegrees(-90);
			continue;
		}
		if (m_state[i] == '-')
		{
			m_direction.RotateVectorByDegrees(90);
			continue;
		}
		//*************End of Dragon Curve Render Logic*************************

		// ************Starmap Render Logic*****************
		// Draw HyperLanes between the stars. 
		if (m_state[i] == 'H')
		{
			SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);
			DrawStraightLine(pRenderer, cameraOffset);
			continue;
		}

		// Setting Draw color for the star then creating the star. I realize I really should overload the SDL_SetRenderDrawColor with my own function that would just take a color struct. 
		// To which I already have a Color.H But I think the next would be to create some static colors since I know their values at run time and use those. I think if I were to keep drawing
		// Lines and such in this manner than I will. but for this assignment I think it's fine. 
		if (m_state[i] == 'L')
		{
			SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
			DrawStarBurst(pRenderer, cameraOffset);
			continue;
		}
		if (m_state[i] == 'Y')
		{
			SDL_SetRenderDrawColor(pRenderer, 196, 180, 84, 255);
			DrawStarBurst(pRenderer, cameraOffset);
			continue;
		}
		if (m_state[i] == 'W')
		{
			SDL_SetRenderDrawColor(pRenderer, 244, 245, 255, 255);
			DrawStarBurst(pRenderer, cameraOffset);
			continue;
		}
		if (m_state[i] == 'O')
		{
			SDL_SetRenderDrawColor(pRenderer, 255, 165, 0, 255);
			DrawStarBurst(pRenderer, cameraOffset);
			continue;
		}

		// [???] I am not sure what I should have done here. I could have made 3 more variables for 15, 30 and 45 but would that make sense? 
		// I could use the StarburstDegreeProgression But I don't think that would have made sense here. What I would have done if you do say that I should have would have been
		// DegreeChanceA, DegreeChanceB etc... and then used them as DegreeChanceA and -DegreeChanceA as a pass in. but I feel like that Kinda makes it less clear to what I am doing. 
		// The thing is that they are somewhat even now just because I wanted something simple but I can randomly assign these or tweak these as needed without cause. 
		if (m_state[i] == '%')
		{
			m_direction.RotateVectorByDegrees(-45);
			continue;
		}
		if (m_state[i] == '$')
		{
			m_direction.RotateVectorByDegrees(45);
			continue;
		}
		if (m_state[i] == '&')
		{
			m_direction.RotateVectorByDegrees(-15);
			continue;
		}
		if (m_state[i] == '*')
		{
			m_direction.RotateVectorByDegrees(15);
			continue;
		}
		if (m_state[i] == '!')
		{
			m_direction.RotateVectorByDegrees(-30);
			continue;
		}
		if (m_state[i] == '@') 
		{
			m_direction.RotateVectorByDegrees(30);
			continue;
		}
		if (m_state[i] == '[') 
		{
			m_positionBuffer.push(m_position);
			continue;
		}
		if (m_state[i] == ']')
		{
			m_position = m_positionBuffer.top();
			m_positionBuffer.pop();
			continue;
		}

		// End of Flower Render Logic
	}

	// Making sure to clear the RenderDrawColor. 
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
}

// Runs the Dragon Curve Rule set. 
void LSystemComponent::RunDragonCurveRules()
{
	std::string newState = "";
	for (size_t i = 0; i < m_state.size(); i++)
	{
		if (m_state[i] == 'F')
		{
			newState.append("F+G");
			continue;
		}
		if (m_state[i] == 'G')
		{
			newState.append("F-G");
			continue;
		}
		if (m_state[i] == '+')
		{
			newState.append("+");
			continue;
		}
		if (m_state[i] == '-')
		{
			newState.append("-");
			continue;
		}
	}
	m_state = newState;
}

// Run Starmap Rule set. 
void LSystemComponent::RunStarmapRules()
{
	std::string newState = "";
	for (size_t i = 0; i < m_state.size(); i++)
	{
		if (m_state[i] == 'R')
		{
			newState.append("HHHHB");
			continue;
		}
		if (m_state[i] == 'B')
		{
			// This is to help understand What is happening
			// newState.append("[?HH^R?][?HH^R?]");
			newState.append("[");
			newState.append(AngleVariationForStarmapRules());
			newState.append("HH");
			newState.append(StarColorPicker());
			newState.append("R");
			newState.append(AngleVariationForStarmapRules());
			newState.append("][");
			newState.append(AngleVariationForStarmapRules());
			newState.append("HH");
			newState.append(StarColorPicker());
			newState.append("R");
			newState.append(AngleVariationForStarmapRules());
			newState.append("]");
			continue;
		}

		if (m_state[i] == '%')
		{
			newState.append("%");
			continue;
		}
		if (m_state[i] == '$')
		{
			newState.append("$");
			continue;
		}
		if (m_state[i] == '[')
		{
			newState.append("[");
			continue;
		}
		if (m_state[i] == ']')
		{
			newState.append("]");
			continue;
		}
		if (m_state[i] == '!')
		{
			newState.append("!");
			continue;
		}
		if (m_state[i] == '@')
		{
			newState.append("@");
			continue;
		}
		if (m_state[i] == '*')
		{
			newState.append("*");
			continue;
		}
		if (m_state[i] == '&')
		{
			newState.append("&");
			continue;
		}
		if (m_state[i] == 'H')
		{
			newState.append("H");
			continue;
		}
		if (m_state[i] == 'T')
		{
			newState.append("T");
			continue;
		}

		// Sun Color constants
		if (m_state[i] == 'Y')
		{
			newState.append("Y");
			continue;
		}
		if (m_state[i] == 'O')
		{
			newState.append("O");
			continue;
		}
		if (m_state[i] == 'L')
		{
			newState.append("L");
			continue;
		}
		if (m_state[i] == 'W')
		{
			newState.append("W");
			continue;
		}

		// This should only Run once so I am sticking it in the bottom. This is to account for the Seed.
		if (m_state[i] == '^')
		{
			newState.append(StarColorPicker());
			continue;
		}
	}
	m_state = newState;
}

// Selects a Angle for the Starmap and all choices have equal chance. 
std::string LSystemComponent::AngleVariationForStarmapRules()
{
	int choice = m_rng.RandomInRange(0, 5);
	switch (choice) 
	{
		case 0: return "%";
		case 1: return "$";
		case 2: return "&";
		case 3: return "!";
		case 4: return "@";
		case 5: return "*";
		default: return "%"; 
	}
}

// Picks a StarColor based of Limits while returning a random range, 
// Most common Star color is White as per google. 
std::string LSystemComponent::StarColorPicker()
{
	float choice = m_rng.NormalizeNoise(m_rng.Get1DNoise(m_rng.RandomInRange(0, 100), kNoiseSeedForStarColorRNG));

	if (choice < kWhiteStarLimit)
		return "W";
	if (choice < kYellowStarLimit)
		return "Y";
	if (choice < kOrangeStarLimit)
		return "O";
	else
		return "L";
}

// Draws lines from a point while rotating around the point. 
void LSystemComponent::DrawStarBurst(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
	for (double i = kStarburstDegreeProgression; i < kStarburstTotalDegrees; i += kStarburstDegreeProgression)
	{
		Vector2<float> flowerTipPosition = m_position + m_direction;
		SDL_RenderDrawLine(pRenderer, static_cast<int>(m_position.x - cameraOffset.x), static_cast<int>(m_position.y - cameraOffset.y), static_cast<int>(flowerTipPosition.x - cameraOffset.x), static_cast<int>(flowerTipPosition.y - cameraOffset.y));
		m_direction.RotateVectorByDegrees(kStarburstDegreeProgression);
	}
}

// Simple helper function to Draw a straight line and move the position forward. 
void LSystemComponent::DrawStraightLine(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
	Vector2<float> newPosition = m_position + m_direction;
	SDL_RenderDrawLine(pRenderer, static_cast<int>(m_position.x - cameraOffset.x), static_cast<int>(m_position.y - cameraOffset.y), static_cast<int>(newPosition.x - cameraOffset.x), static_cast<int>(newPosition.y - cameraOffset.y));
	m_position = newPosition;
}
