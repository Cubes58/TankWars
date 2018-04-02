#pragma once

#include <array>
#include <list>

#include "Node.h"

class Graph : public sf::Drawable {
private:
	static const unsigned int s_m_ColumnSize = 40;
	static const unsigned int s_m_RowSize = 30;
	static const unsigned int s_m_PixelSize = 20;

	std::array<std::array<Node*, s_m_ColumnSize>, s_m_RowSize> m_Grid;
public:
	Graph();
	~Graph();
	
	void draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_State) const;
	void aStarSearchAlgorithm(Node p_CurrentNode, Node p_GoalNode, std::list<Node> p_Path);

	NodeState getNodeState(const sf::Vector2u &p_GridPosition) const;
	void setNodeState(const sf::Vector2u &p_GridPosition, NodeState p_NodeState);
};