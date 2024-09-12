// PathPlanNode.h
#pragma once
#include <float.h>
#include <limits>

class PathPlanNode
{
    int m_tile;
    float m_costSoFar;
    PathPlanNode* m_pPrev;
    bool m_closed;

public:
    PathPlanNode(int tile)
        : m_costSoFar(std::numeric_limits<float>::max())
        , m_pPrev(nullptr)
        , m_closed(false)
        , m_tile(tile)
    {
        //
    }

    // getters
    float GetCost() const { return m_costSoFar; }
    PathPlanNode* GetPrev() const { return m_pPrev; }
    bool IsClosed() const { return m_closed; }
    int GetTile() const { return m_tile; }

    // setters
    void SetCost(float cost) { m_costSoFar = cost; }
    void SetPrev(PathPlanNode* pPrev) { m_pPrev = pPrev; }
    void SetClosed() { m_closed = true; }
};
