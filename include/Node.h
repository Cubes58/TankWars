#pragma once

#include <SFML\Graphics.hpp>

#include "NodeStates.h"

class Node : public sf::Drawable {
private:
	sf::Vector2u m_Position;
	sf::Vector2u m_GridArrayPosition;
	sf::Vector2u m_Size;

	NS::NodeState m_State;

	bool m_Active;
	bool m_Current;

	int m_Parent;

	float m_TotalCost;
	float m_StartDistance;
	float m_GoalDistance;

	sf::RectangleShape m_Shape;
public:
	static const bool s_m_Path = true;
	static const bool s_m_NotPath = false;

	// Constructor.
	Node(const sf::Vector2u &p_Position, const sf::Vector2u &p_Size);
	~Node();

	// Draw methods.
	void draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_States) const;
	int calculateHeuristic(float p_ParentGScore, const sf::Vector2u &p_CurrentPosition, const sf::Vector2u &p_Goal);

	// Accessor methods.
	sf::Vector2u getPosition();

	void setNodeState(NS::NodeState p_NodeState);
	NS::NodeState getNodeState() const;
	inline bool operator<(const Node &p_One) const { return this->m_TotalCost < p_One.m_TotalCost; }
};