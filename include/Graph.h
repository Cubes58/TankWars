#pragma once

#include <array>
#include <list>
#include <map>
#include <algorithm>

#include "Node.h"

class Graph : public sf::Drawable {
private:
	static const unsigned int s_m_ColumnSize = 40;		// Number of columns.
	static const unsigned int s_m_RowSize = 30;			// Number of Rows.
	static const unsigned int s_m_PixelSize = 20;		// Size of the node.

	// Graph nodes.
	std::array<std::array<Node*, s_m_ColumnSize>, s_m_RowSize> m_Nodes;
	// Bases found - storing their positions.
	std::vector<sf::Vector2u> m_BasesFound;
public:
	Graph();
	~Graph();
	
	void draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_State) const;

	// Path finding.
	bool aStarSearchAlgorithm(Node *p_StartNode, Node *p_GoalNode, std::list<Node*> &p_Path);
	std::vector<Node*> getNeighbours(Node &p_Node);
	std::list<Node*> constructPath(Node &p_GoalNode);
	float calculateManhattanHeuristic(Node &p_CurrentNode, Node &p_GoalNode);
	
	// Graph helper methods and accessor methods.
	Node *calculateClosestPathNode(Node *p_GoalNode);
	void setBaseNodes(const sf::Vector2u &p_BasePosition, int p_NeighbourSearchDistance = 40);
	void clearNodes();

	Node &getPixelNode(const sf::Vector2u &p_NodePixelPosition) const;
	Node &getNode(sf::Vector2u &p_NodeGraphPosition);
	Node &getNode(int p_ID) const;

	NodeState getNodeState(sf::Vector2u p_NodeGraphPosition);
	void setNodeState(const sf::Vector2u &p_NodeGraphPosition, const NodeState &p_NodeState);

	std::vector<sf::Vector2u> &getBasesFound();
	bool accountedForBase(const sf::Vector2u &p_Position);
};