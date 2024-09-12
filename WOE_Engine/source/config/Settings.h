#pragma once

constexpr int kMaxWindowHeight = 1080;
constexpr int kMaxWindowWidth = 1920;
constexpr const char* kGameNameString = "MapGeneration";
constexpr const char* kWorldInitDataLocation = "data/WorldData/WorldMapGenerationData.xml";

static constexpr int kTileWidth = 16;
static constexpr int kTileHeight = 16;

// Entities live within Shards. Shards live within a World. World->Shard->Entities.
constexpr unsigned int kNumberOfWorlds = 1;   
constexpr unsigned int kMaxShardsPerWorld = 1;
constexpr unsigned int kMaxEntitiesPerShard = 1;
constexpr int kNumCardinalDirections = 4;
enum CardnalDirection
{
    kWest,
    kNorth,
    kEast,
    kSouth
};
// Alias
using EntityID = unsigned long long int;

// Lock Camera Axis
constexpr bool kYAxisLocked = false;
constexpr bool kXAxisLocked = false;
constexpr const int kXMax = 10000 + (kTileWidth * 500);
constexpr const int kXMin = -10000;
constexpr const int kYMax = 10000 + (kTileHeight * 500);
constexpr const int kYMin = -10000;

// Constant
constexpr int kDefaultSpeed = 50;
constexpr int kDefaultSpeedMultipler = 1;
constexpr float kRectCenteringModifier = 7.5;

//Audio
constexpr bool kAudioEnabled = false;

// Boundaries
constexpr int kLeftBounds = -7;
constexpr int kRightBounds = 485;
constexpr int kFromRightTeleport = 0;
constexpr int kFromLeftTeleport = 470;

// Power-Up
constexpr double g_timerLife = 60;

//Score/Points
static constexpr int kLargeOrbPoints = 100;
static constexpr int kSmallOrbPoints = 10;

// Inputs
// Pressed
constexpr const char* kWPressed = "WPressed";
constexpr const char* kSPressed = "SPressed";
constexpr const char* kAPressed = "APressed";
constexpr const char* kDPressed = "DPressed";
// Released
constexpr const char* kWReleased = "WReleased";
constexpr const char* kSReleased = "SReleased";
constexpr const char* kAReleased = "AReleased";
constexpr const char* kDReleased = "DReleased";

///////////////////////////////////////////////////////////////
//TODO TASK BOARD: 
// Get collision revamped using Collision components and a collision Referee. 
// Rework tilemap for on the fly custom matches and maybe create a feature to change 
//	the board and then write the file. 
// Create a factory for Entity creation and Resource creation. 
///////////////////////////////////////////////////////////////
/*
        <Entity>
            <Transform>
                <position x = "400" y = "300"/>
            </Transform>
            <Components>
                <EnemyMoveComponent>
                </EnemyMoveComponent>
                <Image>
                    <imagepath>data/Textures/RedGhost.png</imagepath>
                    <height height = "15"/>
                    <width width = "15"/>
                </Image>
                <Collider>
                    <height height = "15"/>
                    <width width = "15"/>
                    <message>Ghost</message>
                </Collider>
            </Components>
        </Entity>

        <Transform>
                <position x = "35" y = "35"/>
            </Transform>
            <Components>
                <Collider>
                    <height height = "15"/>
                    <width width = "15"/>
                    <message>PacMan</message>d
                </Collider>
                <PlayerMoveComponent>
                </PlayerMoveComponent>
                <RenderRect>
                    <height height = "15"/>
                    <width width = "15"/>
                    <Color>
                        <g g = "0"/>
                        <b b = "255"/>
                        <r r = "0"/>
                        <a a = "0"/>
                    </Color>
                </RenderRect>
            </Components>
        </Entity>








// From Click Movement for side scroll game.
//THIS CODE IS NEEDED TO MOVE BUT I HAVEN'T TAKEN THE TIME TO STORE IT ELSEWHERE.
//// check to see if we've arrived
//Vec2 delta = m_targetLocation - GetPos();
//if (delta.Length() <= kPathingNodeTolerance)
//{
//	return;
//}
//// step the game object towards their goal. we have not updated yet.
//Vec2 direction = m_targetLocation - GetPos();
//direction.Normalize();
//Vec2 deltaPosition = direction * kDefaultSpeed * (float)deltaTimeMs;
//
//auto pos = GetPos();
//// check to see if we are a flyer and or ground and if we are outside our play area.
//if (m_flyer)
//{
//	bool clean = true;

//	if (GetPos().x < kLowX)
//	{
//		m_pOwner->SetLocation(Vec2(kLowX, GetPos().y));
//		clean = false;
//	}
//	if (GetPos().x > kMaxX)
//	{
//		m_pOwner->SetLocation(Vec2(kMaxX, GetPos().y));
//		clean = false;
//	}
//	if (GetPos().y < kLowY)
//	{
//		m_pOwner->SetLocation(Vec2(GetPos().x, kLowY));
//		clean = false;
//	}
//	if (GetPos().y > kMaxY)
//	{
//		m_pOwner->SetLocation(Vec2(GetPos().x, kMaxY));
//		clean = false;
//	}
//	if (!clean)
//		return;
//
//}
//else
//{
//	bool clean = true;

//	if (GetPos().x < kLowX)
//	{
//		m_pOwner->SetLocation(Vec2(kLowX, GetPos().y));
//		clean = false;
//	}
//	if (GetPos().x > kMaxX)
//	{
//		m_pOwner->SetLocation(Vec2(kMaxX, GetPos().y));
//		clean = false;
//	}
//	if (GetPos().y < kFenceYLimit)
//	{
//		m_pOwner->SetLocation(Vec2(GetPos().x, kFenceYLimit));
//		clean = false;
//	}
//	if (GetPos().y > kMaxY)
//	{
//		m_pOwner->SetLocation(Vec2(GetPos().x, kMaxY));
//		clean = false;
//	}
//	if (!clean)
//		return;
//}

//m_pOwner->UpdateLocation(deltaPosition);


*/