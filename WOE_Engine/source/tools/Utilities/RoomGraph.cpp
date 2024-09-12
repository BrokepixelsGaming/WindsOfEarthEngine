#include "RoomGraph.h"

RoomGraph::~RoomGraph()
{
	
	for (auto node : m_RoomNodes)
	{
		node->m_owningPartian = nullptr;
		delete node;
	}
}
