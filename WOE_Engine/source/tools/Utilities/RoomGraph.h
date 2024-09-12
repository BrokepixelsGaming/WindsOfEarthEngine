#pragma once

#include <vector>
#include "StringId.h"
#include "Rect.h" 
//#include "../../Winds of Earth/source/Components/DiggerComponent.h"

class BSPTreeNode;

class RoomNode
{
public:
    int m_Id = INT32_MAX; // Max is invalid. 
    Rect m_roomRect = {0,0,0,0};
    BSPTreeNode* m_owningPartian = nullptr;
    std::vector<int> m_connectedRoomIds;
    StringId roomType = "Empty";
};

class RoomGraph
{
public:
    ~RoomGraph();
    std::vector<RoomNode*> m_RoomNodes;

};