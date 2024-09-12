#pragma once
#include "ComponentSystem\Component.h"
#include "TileMapSystem/PathPlan.h"
#include "EntitySystem/Entity.h"
#include "tools/Utilities/XorShift.h"
#include "TileMapSystem/TileTypes.h"
#include "tools/Utilities/Rect.h"
#include "tools/Utilities/RoomGraph.h"
#include "tools/Utilities/Vector.h"


// Leaving it inside this class 
class BSPTreeNode
{
    
public:
    
    BSPTreeNode* m_parent = nullptr;
    BSPTreeNode* m_left = nullptr;
    BSPTreeNode* m_right = nullptr;
    bool m_splitVert = false;
    Rect m_area;
    Rect m_roomArea = {0,0,0,0};

    BSPTreeNode(Rect originRect) { m_area = originRect; };
    ~BSPTreeNode();
    int GetAreaSize() { return m_area.height * m_area.width; };
    // render for debug
    void Render([[maybe_unused]] SDL_Renderer* pRenderer, [[maybe_unused]] Vec2 cameraOffset);
    void Split(XorShift* pRng, bool splitVert);
    BSPTreeNode* GetSibling();
};



class DiggerComponent : public Component
{

protected:
	StringId m_type = "DiggerComponent";
	Entity* m_pOwner;

    Plan m_path;
    BSPTreeNode* m_pRoot;
    RoomGraph* m_roomGraph;
    int nodeCount = 0;
    
    CardnalDirection m_direction;
    int m_turnChance;
    int m_roomChance;
    int m_roomsCreated;
    bool m_hasSpawnPlayer;
    bool m_splitVert = false;
    bool m_haveCalledFunction = false;
    Vec2 m_startingLocation;

    XorShift m_rng;

public:
    DiggerComponent(Entity* owner);
    ~DiggerComponent();
    // update functions
    void Update(double deltaTimeMs) override;
    void Render([[maybe_unused]] SDL_Renderer* pRenderer, [[maybe_unused]] Vec2 cameraOffset);

    // accessors
    void SetPath(const Plan& path);
    const Vec2& GetPos() const { return m_pOwner->GetLocation(); };
    void GenerateDungeon();

private:
    void StepMovement(double deltaTimeMs);
    void RandomlyChangeDirections();
    bool CheckChance(int chanceToBeat);
    void DigARoom();
    void DigARoomViaNode(BSPTreeNode* pNode);
    void DigTile(Vec2 location, TileType type);
    bool IsDone();
    void PlaceDoors();
    void Split(BSPTreeNode* pNode);
    void GenerateRooms(BSPTreeNode* pNode);
    void ConnectRooms(BSPTreeNode* pNode);
    void CalculateParentRoomAreas(BSPTreeNode* pNode);
    void CalculateParentsHelper(BSPTreeNode* pNode);
    void DigATunnel(BSPTreeNode* node, BSPTreeNode* sibling);
    void DigABentTunnel(BSPTreeNode* node, BSPTreeNode* sibling);

    // Will move this later. 
    template<typename Type>
    Type Max(Type a, Type b)
    {
        if (a > b)
            return a;
        return b;
    }

    // Will move this later. 
    template<typename Type>
    Type Min(Type a, Type b)
    {
        if (a < b)
            return a;
        return b;
    }
};
