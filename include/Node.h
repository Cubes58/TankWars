#pragma once

#include <SFML\Graphics.hpp>
#include <cmath>

enum class NodeState : unsigned int {		/* Must explicitly type cast! */
	PATH,
	WALL,
	GOAL,
	START,
	OPEN,
	CLOSED,
	CURRENT,
	BASE
};

class Node : public sf::Drawable {
private:
	sf::Vector2u m_Position;
	sf::Vector2u m_GraphArrayPosition;
	sf::Vector2u m_Size;

	NodeState m_State;

	sf::Vector2u m_ParentNodeGraphArrayPosition;	// Node you were at before this one.

	bool m_Active;
	bool m_Current;
	
	float m_GValue;		// Distance cost of current node to start node.
	float m_HValue;		// Distance cost of current node to goal node.
	float m_FValue;		// Total distance cost, of the node.

	sf::RectangleShape m_Shape;
public:
	static const bool s_m_Path = true;
	static const bool s_m_NotPath = false;

	// Constructor.
	Node() = default;
	Node(const sf::Vector2u &p_Position, const sf::Vector2u &p_Size, const sf::Vector2u &p_GraphArrayPosition, NodeState p_State = NodeState::OPEN);
	~Node();

	bool checkValidNode();

	// Draw methods.
	void draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_States) const;
	int calculateManhattanHeuristic(Node &p_PreviousNode, Node &p_GoalNode);

	// Accessor methods.
	sf::Vector2u getPosition();

	void setNodeState(NodeState p_NodeState);
	NodeState getNodeState() const;

	void setParentNodeGraphArrayPosition(const sf::Vector2u &p_ParentNodeGraphArrayPosition);
	sf::Vector2u getParentNodeGraphArrayPosition();

	sf::Vector2u getGraphArrayPosition();

	void setGValue(float p_GValue);
	float getGValue();

	void setHValue(float p_HValue);
	float getHValue();

	void setFValue(float p_FValue);
	float getFValue();

	// Compare nodes based on their m_FValue - sort by lowest f cost use the less than operator to compare.
	inline bool operator<(const Node &p_Other) const { 
		return this->m_FValue < p_Other.m_FValue; 
	}

	inline bool operator==(const Node &p_Other) const {
		return this->m_Position == p_Other.m_Position;
	}
	
};