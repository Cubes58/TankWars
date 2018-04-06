#pragma once

#include <array>
#include <list>
#include <iostream>
#include <algorithm>

#include "Node.h"

class Graph : public sf::Drawable {
private:
	static const unsigned int s_m_ColumnSize = 40;
	static const unsigned int s_m_RowSize = 30;
	static const unsigned int s_m_PixelSize = 20;

	std::array<std::array<Node*, s_m_ColumnSize>, s_m_RowSize> m_Nodes;
public:
	Graph();
	~Graph();
	
	void draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_State) const;

	bool aStarSearchAlgorithm(Node &p_StartNode, Node &p_GoalNode, std::list<Node*> &p_Path);
	std::vector<Node*> getNeighbours(Node &p_Node);
	std::list<Node*> constructPath(Node &p_GoalNode, std::map<int, Node>);

	void clearNodes();

	Node *getPixelNode(const sf::Vector2u &p_NodePixelPosition);
	Node &getNode(const sf::Vector2u &p_NodeGraphPosition);
	Node &getNode(int id) const;

	void setNodeState(const sf::Vector2u &p_GridPosition, NodeState p_NodeState);
	NodeState getNodeState(const sf::Vector2u &p_GridPosition) const;
};