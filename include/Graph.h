#pragma once

#include <array>

#include "Node.h"

class Graph : public sf::Drawable {
private:
	static const unsigned int s_m_ColumnSize = 10;
	static const unsigned int s_m_RowSize = 10;

	std::array<std::array<Node, s_m_ColumnSize>, s_m_RowSize> m_Grid;
public:
	Graph();
	~Graph();

	void draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_State) const;

	NodeState getNodeState() const;
	void setNodeState(NodeState p_NodeState);
};