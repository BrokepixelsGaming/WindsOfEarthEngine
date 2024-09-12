#include "ScoreBoardComponent.h"

ScoreBoardComponent::ScoreBoardComponent(Entity* pOwner, SDL_FRect location)
	: m_pOwner(pOwner)
	, m_origin(location)
	, m_currentScoreCount(0)
	, m_collector(nullptr)
{
	m_pFont = TTF_OpenFont("data/Fonts/arial.ttf", 120);
	std::string count = "Score:: ";
	count.append(std::to_string(m_currentScoreCount));

	//std::string instructions = "****Movement****\nW == UP\nA == LEFT\nS == DOWN\nD == RIGHT\nESC == Quit\n";
	std::string movementString = "****Movement****";
	std::string wUpString =      "    W == Up";
	std::string aLeftString =    "    A == Left";
	std::string sDownString =    "    S == Down";
	std::string dRightString =   "    D == Right";
	std::string escQuitString =  "    ESC == Quit";

	constexpr int kSpacing = 75;
	constexpr float kHeight = 75;
	constexpr float kWidth = 250;
	// take this out. TODO::
	for (auto& textField : m_textFields)
	{
		textField = nullptr;
	}
	auto pRenderer = EngineSingleton::Get()->GetWindow()->GetRenderer();

	m_textFields[0] = BLEACH_NEW(UITextField)(m_pFont, movementString.c_str(), SDL_Color{0, 150, 255, 255}, SDL_FRect{m_origin.x, m_origin.y, kWidth, kHeight }, pRenderer);
	m_textFields[1] = BLEACH_NEW(UITextField)(m_pFont, wUpString.c_str(), SDL_Color{ 0,255,0,255 }, SDL_FRect{ m_origin.x, m_origin.y + (kSpacing),kWidth, kHeight }, pRenderer);
	m_textFields[2] = BLEACH_NEW(UITextField)(m_pFont, aLeftString.c_str(), SDL_Color{ 0,255,0,255 }, SDL_FRect{ m_origin.x, m_origin.y + (kSpacing * 2), kWidth, kHeight }, pRenderer);
	m_textFields[3] = BLEACH_NEW(UITextField)(m_pFont, sDownString.c_str(), SDL_Color{ 0,255,0,255 }, SDL_FRect{ m_origin.x, m_origin.y + (kSpacing * 3), kWidth, kHeight }, pRenderer);
	m_textFields[4] = BLEACH_NEW(UITextField)(m_pFont, dRightString.c_str(), SDL_Color{ 0,255,0,255 }, SDL_FRect{ m_origin.x, m_origin.y + (kSpacing * 4), kWidth, kHeight }, pRenderer);
	m_textFields[5] = BLEACH_NEW(UITextField)(m_pFont, escQuitString.c_str(), SDL_Color{ 102,0,0,255 }, SDL_FRect{ m_origin.x, m_origin.y + (kSpacing * 5),kWidth, kHeight }, pRenderer);
	m_textFields[6] = BLEACH_NEW(UITextField)(m_pFont, count.c_str(), SDL_Color{ 0,255,0,255 }, SDL_FRect{ m_origin.x - 450, m_origin.y + (kSpacing * 7) }, pRenderer);
	m_scoreField = m_textFields[6];


}

ScoreBoardComponent::~ScoreBoardComponent()
{
	//BLEACH_DELETE(m_scoreField);
	for (auto& textField : m_textFields)
	{
		if (textField != nullptr)
			BLEACH_DELETE(textField);
	}
	m_collector = nullptr;
	m_scoreField = nullptr;
}

void ScoreBoardComponent::Render(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
	for (auto& textField : m_textFields)
	{
		if (textField != nullptr)
			textField->Render(pRenderer, cameraOffset);
	}
}

void ScoreBoardComponent::Update([[maybe_unused]] double deltaTime)
{
	// We want to make sure we have a valid Collector.
	if (m_collector == nullptr)
	{
		auto& listOfEntities = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager()->GetAllEntities();

		for (auto entity : listOfEntities)
		{
			// Here we are just looking for the Entity with Collector, this is assumed that there will ever only be one.
			auto collector = entity->GetComponent<CollectorComponent>("Collector");
			if (collector != nullptr)
			{
				// here we will store the pointer to the collecter that we are going to monitor. 
				m_collector = collector;
				std::string count = "Score:: ";
				count.append(std::to_string(m_currentScoreCount));

				m_scoreField->UpdateText(count.c_str());

				// We adjust the size to account for the length of the string that way we don't distort it as we grow. 
				float newWidth = static_cast<float>(count.size()) * 25;
				m_scoreField->SetHeightWidth(35, newWidth);
				break;
			}
		}
		// Error checking. If we fail this is how we handle it. 
		if (m_collector == nullptr)
		{
			//std::cout << "Error finding the collector.\n";
			return;
		}
	}

	// 1: case the score has not changed.
	// nothing
	
	// 2: case the score has changed. 
	if (m_currentScoreCount != m_collector->GetCount())
	{
		//first we will update the current count.
		m_currentScoreCount = m_collector->GetCount();
		// here we update the score count and volume. 
		std::string count = "Score:: ";
		count.append(std::to_string(m_currentScoreCount));

		m_scoreField->UpdateText(count.c_str());

		// We adjust the size to account for the length of the string that way we don't distort it as we grow. 
		float newWidth = static_cast<float>(count.size()) * 25;
		m_scoreField->SetHeightWidth(35, newWidth);
		//
	}
}
