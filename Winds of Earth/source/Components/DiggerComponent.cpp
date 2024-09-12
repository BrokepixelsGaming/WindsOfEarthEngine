#include "DiggerComponent.h"
#include "EngineInterface/EngineSingleton.h"
#include "ClickMovement.h"
#include <vector>
#include <cmath>
//#include <list>
#include <stack>
#include <set>
//#include <iostream>

//////////////////////////////////////////////
// Tuneable Values for Digger. 
constexpr int kRoomsToCreate = 6;
constexpr int kRoomMinWidthSize = 2;
constexpr int kRoomMinHeightSize = 2;
constexpr int kRoomMaxWidthSize = 8;
constexpr int kRoomMaxHeightSize = 8;
constexpr int kRoomChanceAdditive = 2;
constexpr int kTurnChanceAdditive = 6;
///////////////////////////////////////////////
static constexpr float kSpeed = 10.0f;
static constexpr float kPathingNodeTolerance = 0.5f;



DiggerComponent::DiggerComponent(Entity* owner)
    : m_pOwner(owner)
    , m_turnChance(0)
    , m_roomChance(0)
    , m_roomsCreated(0)
    , m_hasSpawnPlayer(false)
    , m_pRoot(nullptr)
    , m_roomGraph(nullptr)
{
    //auto position = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetRandomWalkableTilePos();
    m_pOwner->SetLocation(Vec2{ 8, 8 });
    m_startingLocation = Vec2{ 8, 8 };

    // Setting the seed to get the same map.
    m_rng.SetSeed(1696657149);
    
    // start with a random direction 
    RandomlyChangeDirections();
    
}

void DiggerComponent::Update([[maybe_unused]]double deltaTimeMs)
{
    if(!m_haveCalledFunction)
    {
        GenerateDungeon();
        m_haveCalledFunction = true;
    }


    //if (!IsDone()) 
    //{
    //    if (!m_path.empty())
    //    {
    //        StepMovement(deltaTimeMs);
    //    }
    //    else
    //    {
    //        // check if we need to change directions.
    //        if (CheckChance(m_turnChance))
    //        {
    //            RandomlyChangeDirections();
    //            m_turnChance = 0;
    //        }
    //        else
    //            m_turnChance += kTurnChanceAdditive;

    //        // check if we need to build a room. 
    //        if (CheckChance(m_roomChance))
    //        {
    //            DigARoom();
    //            m_roomChance = 0;
    //        }
    //        else
    //            m_roomChance += kRoomChanceAdditive;
    //        // check in the direction we are traveling for a wall. 
    //        // Get our current location.
    //        Vec2 nextTile = m_pOwner->GetLocation();
    //        //Check and add our travel direction. 
    //        switch (m_direction) 
    //        {
    //            case kWest:     nextTile += {-kTileWidth, 0};   break;
    //            case kNorth:    nextTile += {0, kTileHeight};   break;
    //            case kEast:     nextTile += {kTileWidth, 0};    break;
    //            case kSouth:    nextTile += {0, -kTileHeight};  break;
    //        }
    //        // Check the tile type if it's a wall we need to change it. 
    //        auto tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(nextTile.x, nextTile.y);
    //        // if there is a wall we are going to dig it.
    //        if (tileType == "wall")
    //        {
    //            DigTile(nextTile, TileType::kEmptyPath);
    //        }

    //        // if there is not a wall we are just going to walk to the next tile.
    //        // 
    //        // Get a new path. 
    //        auto path = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GeneratePathToBestTarget(m_pOwner->GetLocation(), nextTile);
    //        SetPath(path);
    //    }
    //}
    //else
    //{
    //    // I only want to spawn one fighter. 
    //    if (!m_hasSpawnPlayer)
    //    {
    //        // spawn another Entity that has click movement. ....
    //        auto pEntity = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetEntityManager()->CreateEntity("Units/Fighter");
    //        pEntity->SetLocation(EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetRandomWalkableTilePos());
    //        pEntity->GetComponent<ClickMovement>("ClickMovement")->StopMovement();
    //        m_hasSpawnPlayer = true;
    //    }
    //}
}

// setting the path for the digger. 
void DiggerComponent::SetPath(const Plan& path)
{
    m_path.clear();
    m_path = path;
}

void DiggerComponent::StepMovement(double deltaTimeMs)
{
    // step the game object towards their goal
    Vec2 direction = m_path.front() - GetPos();
    direction.Normalize();
    Vec2 deltaPosition = direction * kDefaultSpeed * (float)deltaTimeMs;
    m_pOwner->UpdateLocation(deltaPosition);

    // check to see if we've arrived
    Vec2 delta = m_path.front() - GetPos();
    if (delta.Length() <= kPathingNodeTolerance)
        m_path.pop_front();
}

// Checks the chance by seeing if it beats a roll betwen 0-100. if chanceToBeat is higher it returns true. 
bool DiggerComponent::CheckChance(int chanceToBeat)
{
    if(m_rng.RandomInRange(0, 100) < chanceToBeat)
        return true;
    return false;
}

// Makes a Room of a random size based off the min and max of the rooms possibles. 
void DiggerComponent::DigARoom()
{
    // Determine the height and width of the room 
    int roomHeight = m_rng.RandomInRange(kRoomMinHeightSize, kRoomMaxHeightSize);
    int roomWidth = m_rng.RandomInRange(kRoomMinWidthSize, kRoomMaxWidthSize);

    // We will try to locate the top left tile. 
    float towardsTop = (roomHeight - 1.f) / 2;
    float towardsLeft = (roomWidth - 1.f) / 2;

    // This is so I can figure how many steps I need to take towards top left. 
    Vec2 centerOfRoom = m_pOwner->GetLocation();

    // We are starting in the top left. 
    Vec2 pointToDig = centerOfRoom + Vec2{ -(towardsLeft * kTileWidth), -(towardsTop * kTileHeight)};
   
    // Here we dig the room. 
    for (int height = 0; height < roomHeight; ++height) 
    {
        for (int width = 0; width < roomWidth; ++width)
        {
            DigTile(pointToDig, TileType::kEmptyPath);
            pointToDig += {kTileWidth, 0 };
        }
        // Go back to the left.
        pointToDig += {-(kTileWidth * static_cast<float>(roomWidth)), kTileHeight};
    }
    // Reset the room chance. 
    m_roomChance = 0;
    m_roomsCreated++;
}

// Call a function to change the tile Baselayer by changing the TileDef* for the tile. 
void DiggerComponent::DigTile(Vec2 location, TileType type)
{
    EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->SetTileTypeByLocation(location.x, location.y, type);
}

///////////////////////////////////////////////////////////////
// Connecting all the rooms via the Room graph. 
///////////////////////////////////////////////////////////////
void DiggerComponent::ConnectRooms(BSPTreeNode* pNode)
{
    // Build our nodes that way we can walk through them. 
    std::vector<std::set<BSPTreeNode*>> nodesByDepth;
    if (pNode == nullptr)
        return;
    
    
    std::stack<BSPTreeNode*> nodeStack;
    std::stack<int> depthStack;
    
    nodeStack.push(pNode);
    depthStack.push(0);
    
    while (!nodeStack.empty()) 
    {
        BSPTreeNode* currentNode = nodeStack.top();
        nodeStack.pop();
    
        int currentDepth = depthStack.top();
        depthStack.pop();
    
        // Ensure the vector of sets has enough depth levels
        if (currentDepth >= nodesByDepth.size()) 
        {
            nodesByDepth.push_back(std::set<BSPTreeNode*>());
        }
    
        // Add the current node to the set at its depth
        nodesByDepth[currentDepth].insert(currentNode);
    
        // Process child nodes
        if (currentNode->m_left != nullptr) 
        {
            nodeStack.push(currentNode->m_left);
            depthStack.push(currentDepth + 1);
        }
        if (currentNode->m_right != nullptr) 
        {
            nodeStack.push(currentNode->m_right);
            depthStack.push(currentDepth + 1);
        }
    }
    
    for (size_t i = nodesByDepth.size() - 1; i > 0; --i)
    {
        while (!nodesByDepth[i].empty())
        {
            BSPTreeNode* node = *nodesByDepth[i].begin();
            BSPTreeNode* sibling = node->GetSibling();
            if (sibling != nullptr)
            {
                // Quick before the Hyneas come!
                DigATunnel(node, sibling);
    
                nodesByDepth[i].erase(node);
                nodesByDepth[i].erase(sibling);
            }
        }
    }
}

// Returns true when the number of rooms has been reached. 
bool DiggerComponent::IsDone()
{
    if(kRoomsToCreate > m_roomsCreated)
        return false;
    return true;
}

void DiggerComponent::GenerateDungeon()
{
    Vec2 heightAndWidth = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileMapWidthAndHeight();

    // make the root node. 
    m_pRoot = new BSPTreeNode(Rect{ 0, 0, static_cast<int>(heightAndWidth.x), static_cast<int>(heightAndWidth.y) });

    // Generate Room graph. 
    m_roomGraph = new RoomGraph();

    Split(m_pRoot);
    GenerateRooms(m_pRoot);
    CalculateParentRoomAreas(m_pRoot);
    ConnectRooms(m_pRoot);
}

void DiggerComponent::PlaceDoors()
{
    struct node 
    {
        Vec2 location;
        TileType type;
        TileType neighbors[4]; // 0: West 1: North 2: East 3: South
    };
    // 
}

DiggerComponent::~DiggerComponent()
{
    delete m_roomGraph;
    delete m_pRoot;
}

void DiggerComponent::Render(SDL_Renderer* pRenderer, Vec2 cameraOffset)
{
    m_pRoot->Render(pRenderer, cameraOffset);
}

void DiggerComponent::Split(BSPTreeNode* pNode)
{
    typedef std::list<BSPTreeNode*> OpenSet;
    OpenSet openSet;
    nodeCount = 1;
    openSet.push_back(pNode);

    while (!openSet.empty())
    {
        auto leaf = openSet.front();
        
        
        bool choice = m_rng.RandomBool();
        pNode->m_splitVert = choice;
        leaf->Split(&m_rng, choice);
        //m_splitVert = !m_splitVert;
        nodeCount += 2;
        openSet.pop_front();

        if(leaf->m_left != nullptr)
            if (leaf->m_left->m_area.width / 2 > kRoomMaxWidthSize && leaf->m_left->m_area.height / 2 > kRoomMaxHeightSize)
            {
                openSet.push_back(leaf->m_left);
            }
        // If we are splittable
        if (leaf->m_right != nullptr)
            if (leaf->m_right->m_area.width / 2 > kRoomMaxWidthSize && leaf->m_right->m_area.height / 2 > kRoomMaxHeightSize)
            {
                openSet.push_back(leaf->m_right);
            }
    }
}

void DiggerComponent::CalculateParentRoomAreas(BSPTreeNode* pNode)
{
    // Is a leaf Node.
    if (pNode->m_left == nullptr && pNode->m_right == nullptr)
    {
        CalculateParentsHelper(pNode);
    }
    else
    {
        if (pNode->m_left != nullptr)
           CalculateParentRoomAreas(pNode->m_left);
        if (pNode->m_right != nullptr)
           CalculateParentRoomAreas(pNode->m_right);
    }
}

void DiggerComponent::DigARoomViaNode(BSPTreeNode* pNode)
{
    // Get the area in which the node resides.
    
    int roomHeight = -1;
    int roomWidth = -1;
        // Get a random height and width of a room that would fit within the bounds. 
        // IE don't dig 5 width in an area of 3 width. aka if at min just dig min. 
         // Determine the height and width of the room 
    roomHeight = m_rng.RandomInRange(kRoomMinHeightSize, kRoomMaxHeightSize);
    roomWidth = m_rng.RandomInRange(kRoomMinHeightSize, kRoomMaxHeightSize);

    // we are checking and clamping the height and width to what the node will allow. so if only 3 is available and a 5 is chosen.
    // It will reassign to a 3.
    if (pNode->m_area.height < roomHeight)
        roomHeight = pNode->m_area.height;
    if (pNode->m_area.width < roomWidth)
        roomWidth = pNode->m_area.width;
    if (roomHeight < 0)
        roomHeight = kRoomMinHeightSize;
    if (roomWidth < 0 )
        roomWidth = kRoomMinWidthSize;

    int xOffset = 0;
    int yOffset = 0;
    // Generate a roomNode
    m_roomGraph->m_RoomNodes.push_back(new RoomNode());
    RoomNode* pRoomNode = m_roomGraph->m_RoomNodes.back();

    // assign a room number. and Parent Partian. 
    m_roomsCreated++;
    pRoomNode->m_Id = m_roomsCreated;
    pRoomNode->m_owningPartian = pNode;

    // now place the room within the bounds. 
    if (pNode->m_area.width > roomWidth)
    {
        // taking the difference and handling a offset
        xOffset = m_rng.RandomInRange(0, (pNode->m_area.width - roomWidth));
    }
    if (pNode->m_area.height > roomHeight)
    {
        yOffset = m_rng.RandomInRange(0, (pNode->m_area.height - roomHeight));
    }

    // Dig a room out based on the location of the tiles. 
    // We are starting in the top left. 
    Vec2 pointToDig = {(static_cast<float>((pNode->m_area.x + xOffset) * kTileWidth)), static_cast<float>((pNode->m_area.y + yOffset) * kTileHeight)};
    pRoomNode->m_roomRect = { (static_cast<int>(pointToDig.x)), (static_cast<int>(pointToDig.y)), roomHeight, roomWidth };
    pNode->m_roomArea = pRoomNode->m_roomRect;

    // Here we dig the room. 
    for (int height = 0; height < roomHeight; ++height)
    {
        for (int width = 0; width < roomWidth; ++width)
        {
            DigTile(pointToDig, TileType::kEmptyPath);
            // Add tiles to the RoomNode             
            pointToDig += {kTileWidth, 0 };
        }
        // Go back to the left.
        pointToDig += {-(kTileWidth * static_cast<float>(roomWidth)), kTileHeight};
    }
    
}

// Only Leaf nodes should call this. 
void DiggerComponent::CalculateParentsHelper(BSPTreeNode* pNode)
{
    while (pNode->m_parent != nullptr)
    {
        // Move to the parent of the leaf node. 
        pNode = pNode->m_parent;

        auto* leftLeaf = pNode->m_left;
        auto* rightLeaf = pNode->m_right;

        // Take the smallest X. and calculate the width. 
        if (leftLeaf->m_roomArea.x < rightLeaf->m_roomArea.x)
        {
            pNode->m_roomArea.x = leftLeaf->m_roomArea.x;
            pNode->m_roomArea.width = rightLeaf->m_roomArea.x - leftLeaf->m_roomArea.x;
        }

        if (leftLeaf->m_roomArea.x > rightLeaf->m_roomArea.x)
        {
            pNode->m_roomArea.x = rightLeaf->m_roomArea.x;
            pNode->m_roomArea.width = leftLeaf->m_roomArea.x - rightLeaf->m_roomArea.x;
        }

        // Take the smallest Y.
        if (leftLeaf->m_roomArea.y < rightLeaf->m_roomArea.y) 
        {
            pNode->m_roomArea.y = leftLeaf->m_roomArea.y;
            pNode->m_roomArea.height = rightLeaf->m_roomArea.y - leftLeaf->m_roomArea.y;
        }
        if (leftLeaf->m_roomArea.y > rightLeaf->m_roomArea.y) 
        {
            pNode->m_roomArea.y = rightLeaf->m_roomArea.y;
            pNode->m_roomArea.height = leftLeaf->m_roomArea.y - rightLeaf->m_roomArea.y;
        }
    }
}

// recursively call GenerateRooms till leaf nodes then will call DigARoomViaNode if it is a leaf.
void DiggerComponent::GenerateRooms(BSPTreeNode* pNode)
{
    if (pNode == nullptr)
        return;

    // Is a leaf Node.
    if (pNode->m_left == nullptr && pNode->m_right == nullptr)
    {
        DigARoomViaNode(pNode);
    }
    else
    {
        if (pNode->m_left != nullptr)
            GenerateRooms(pNode->m_left);
        if (pNode->m_right != nullptr)
            GenerateRooms(pNode->m_right);
    }
}

void DiggerComponent::DigABentTunnel(BSPTreeNode* pNode, BSPTreeNode* pSibling)
{
    int xDirection = pNode->m_roomArea.x < pSibling->m_roomArea.x ? 1 : -1;
    int yDirection = pNode->m_roomArea.y < pSibling->m_roomArea.y ? 1 : -1;

    if (true)
    {
        float startX = xDirection < 0 ? static_cast<float>(pNode->m_roomArea.x) : static_cast<float>(pNode->m_roomArea.x + pNode->m_roomArea.width);
        float startY = static_cast<float>(m_rng.RandomInRange(pNode->m_roomArea.y, pNode->m_roomArea.y + pNode->m_roomArea.height));
        float endX = static_cast<float>(m_rng.RandomInRange(pSibling->m_roomArea.x, pSibling->m_roomArea.x + pSibling->m_area.width));
        //float endY = yDirection < 0 ? static_cast<float>(pSibling->m_roomArea.y + pSibling->m_roomArea.height) : static_cast<float>(pSibling->m_roomArea.y);
        
        auto tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(startX, startY);

        if(xDirection == -1)
            while (startX > endX)
            {

                DigTile({ startX, startY }, TileType::kEmptyPath);
                startX += kTileWidth * xDirection;
            }
        else
            while (startX < endX + kTileWidth)
            {
                DigTile({ startX, startY }, TileType::kEmptyPath);
                startX += kTileWidth * xDirection;
            }

        startY += kTileHeight * yDirection;
        tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(endX, startY);
        while (tileType == "wall")
        {
            DigTile({ endX, startY }, TileType::kEmptyPath);
            startY += kTileHeight * yDirection;
            tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(endX, startY);
        }
    }
    //else
    //{
    //    float startX = xDirection < 0 ? pNode->m_roomArea.x : pNode->m_roomArea.x + pNode->m_roomArea.width;
    //    float startY = m_rng.RandomInRange(pNode->m_roomArea.y, pNode->m_roomArea.y + pNode->m_roomArea.height);
    //    float endX = m_rng.RandomInRange(pSibling->m_roomArea.x, pSibling->m_roomArea.x + pSibling->m_area.width);
    //    float endY = yDirection < 0 ? pSibling->m_roomArea.y + pSibling->m_roomArea.height : pSibling->m_roomArea.y;

    //    while (startY < endY)
    //    {
    //        DigTile({ startX, startY }, TileType::kEmptyPath);
    //        startY += kTileHeight * yDirection;
    //    }

    //    endX += kTileWidth * xDirection;
    //    auto tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(endX, startY);
    //    while (tileType == "wall")
    //    {
    //        DigTile({ startX, endY }, TileType::kEmptyPath);
    //        endX += kTileWidth * xDirection;
    //        tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(endX, endY);
    //    }
    //}
}

void DiggerComponent::DigATunnel(BSPTreeNode* pNode, BSPTreeNode* pSibling)
{
    if (pNode->m_parent->m_splitVert)
    {
        int leftFurthestToRight = Max(pNode->m_roomArea.x, pSibling->m_roomArea.x);
        int rightFurthestToLeft = Min(pNode->m_roomArea.x + pNode->m_roomArea.width, pSibling->m_roomArea.x + pSibling->m_roomArea.width);

        if (rightFurthestToLeft < leftFurthestToRight)
        {
            DigABentTunnel(pNode, pSibling);
        }
        else
        {
            float startX = static_cast<float>(m_rng.RandomInRange( rightFurthestToLeft, leftFurthestToRight));
            float startY = static_cast<float>(Min(pNode->m_roomArea.y + pNode->m_roomArea.height, pSibling->m_roomArea.y + pSibling->m_roomArea.height));

            startY += kTileHeight;
            auto tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(startX, startY);
            while (tileType == "wall")
            {
                DigTile({ startX, startY }, TileType::kEmptyPath);
                startY += kTileHeight;
                tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(startX, startY);
            }
        }
    }
    else
    {
        int topFurthestToBottom = Max(pNode->m_roomArea.y, pSibling->m_roomArea.y);
        int bottomFurthestToTop = Min(pNode->m_roomArea.y + pNode->m_roomArea.height, pSibling->m_roomArea.y + pSibling->m_roomArea.height);

        if (bottomFurthestToTop < topFurthestToBottom)
        {
            DigABentTunnel(pNode, pSibling);
        }
        else
        {
            float startX = static_cast<float>(Min(pNode->m_roomArea.x + pNode->m_roomArea.width, pSibling->m_roomArea.x + pSibling->m_roomArea.width));
            float startY = static_cast<float>(m_rng.RandomInRange(topFurthestToBottom, bottomFurthestToTop));

            startX += kTileWidth;
            auto tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(startX, startY);
            while (tileType == "wall")
            {
                DigTile({ static_cast<float>(startX), static_cast<float>(startY) }, TileType::kEmptyPath);
                startX += kTileWidth;
                tileType = EngineSingleton::Get()->GetWorldManager()->GetCurrentWorld()->GetTileTypeByLocation(startX, startY);
            }
        }
    }
}

// Uses xorShift to change in a random direction. I am also limiting it to not allow a turn back the opposite direction.
void DiggerComponent::RandomlyChangeDirections()
{   
    // Here I prevent the direction from going in the direct opposite way. 
    bool chosen = false; 
    int choice = m_rng.RandomInRange(0, 3);
    while (!chosen) 
    {
        if (choice == 0 && m_direction == kEast)
        {
            choice = m_rng.RandomInRange(0, 3);
            continue;
        }   
        if (choice == 1 && m_direction == kSouth) 
        {
            choice = m_rng.RandomInRange(0, 3);
            continue;
        }
        if (choice == 2 && m_direction == kWest) 
        {
            choice = m_rng.RandomInRange(0, 3);
            continue;
        }
        if (choice == 3 && m_direction == kNorth) 
        {
            choice = m_rng.RandomInRange(0, 3);
            continue;
        }
        // sets to leave the while loop. 
        chosen = true;
    }
    // Setting the direction of travel. 
    assert(choice >= 0 && choice < kNumCardinalDirections);
    m_direction = static_cast<CardnalDirection>(choice);
}

void BSPTreeNode::Split(XorShift* pRng, bool splitVert)
{
        
    if (splitVert && m_area.width / 2 > kRoomMaxWidthSize)
    {
        int low = m_area.x + kRoomMinWidthSize;
        int high = (m_area.x + m_area.width) - kRoomMinWidthSize;
        int splitX = pRng->RandomInRange(low, high);

        // assign left and right
        m_left = new BSPTreeNode(Rect{ m_area.x, m_area.y, m_area.height, (splitX - m_area.x) });
        m_left->m_parent = this;
        m_right = new BSPTreeNode(Rect{ m_area.x + splitX, m_area.y, m_area.height, (m_area.width - (m_area.x + splitX)) });
        m_right->m_parent = this;
    }
    if(!splitVert && m_area.height / 2 > kRoomMaxHeightSize)
    {
        int low = m_area.y + kRoomMinHeightSize;
        int high = (m_area.y + m_area.height) - kRoomMinHeightSize;
        int splitY = pRng->RandomInRange(low, high);

        // assign Top and Bottom
        m_left = new BSPTreeNode(Rect{ m_area.x, m_area.y, splitY - m_area.y, m_area.width});
        m_left->m_parent = this;
        m_right = new BSPTreeNode(Rect{ m_area.x, splitY, (m_area.height - m_left->m_area.height), m_area.width});
        m_right->m_parent = this;
    }
}

///////////////////////////////////////////////////////////////
// Destructor for BSPTreeNodes
///////////////////////////////////////////////////////////////
BSPTreeNode::~BSPTreeNode()
{
    if (m_left != nullptr)
    {
        delete m_left;
        m_left = nullptr;
    }
    if (m_right != nullptr)
    {
        delete m_right;
        m_right = nullptr;
    }
    m_parent = nullptr;
}

BSPTreeNode* BSPTreeNode::GetSibling()
{
    if (m_parent->m_left == this)
        return m_parent->m_right;
    else
        return m_parent->m_left;
}

void BSPTreeNode::Render([[maybe_unused]] SDL_Renderer* pRenderer, [[maybe_unused]] Vec2 cameraOffset)
{
    Vec2 topLeft = { static_cast<float>(m_area.x * kTileWidth) , static_cast<float>(m_area.y) * kTileHeight };
    Vec2 topRight = { static_cast<float>(m_area.x + m_area.width) * kTileWidth, static_cast<float>(m_area.y) * kTileHeight };
    Vec2 bottomLeft = { static_cast<float>(m_area.x) * kTileWidth, static_cast<float>(m_area.y + m_area.height) * kTileHeight };
    Vec2 bottomRight = { static_cast<float>(m_area.x + m_area.width) * kTileWidth, static_cast<float>(m_area.y + m_area.height) * kTileHeight};


    SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
    // top left corner to bottom left. 
    SDL_RenderDrawLine(pRenderer, static_cast<int>(topLeft.x - cameraOffset.x), static_cast<int>(topLeft.y - cameraOffset.y), static_cast<int>(bottomLeft.x - cameraOffset.x), static_cast<int>(bottomLeft.y - cameraOffset.y));
    // top Left corner to top Right
    SDL_RenderDrawLine(pRenderer, static_cast<int>(topLeft.x - cameraOffset.x), static_cast<int>(topLeft.y - cameraOffset.y), static_cast<int>(topRight.x - cameraOffset.x), static_cast<int>(topRight.y - cameraOffset.y));
    // top right corner to bottom Right
    SDL_RenderDrawLine(pRenderer, static_cast<int>(topRight.x - cameraOffset.x), static_cast<int>(topRight.y - cameraOffset.y), static_cast<int>(bottomRight.x - cameraOffset.x), static_cast<int>(bottomRight.y - cameraOffset.y));
    // bottom right corner to bottom left
    SDL_RenderDrawLine(pRenderer, static_cast<int>(bottomRight.x - cameraOffset.x), static_cast<int>(bottomRight.y - cameraOffset.y), static_cast<int>(bottomLeft.x - cameraOffset.x), static_cast<int>(bottomLeft.y - cameraOffset.y));
    
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

    if (m_left)
        m_left->Render(pRenderer, cameraOffset);
    if (m_right)
        m_right->Render(pRenderer, cameraOffset);

}
