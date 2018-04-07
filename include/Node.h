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
	BASE,
	NOTHING
};

class Node : public sf::Drawable {
private:
	int m_ID;
	sf::Vector2u m_PixelPosition;		// Its position on the screen.
	sf::Vector2u m_GraphArrayPosition;	// Its position in the (Graph) array.
	sf::Vector2u m_Size;

	NodeState m_State;

	bool m_IsPath;
	
	float m_GValue;			// Distance cost of current node to start node.
	float m_HValue;			// Distance cost of current node to goal node.
	float m_FValue;			// Total distance cost, of the node.
	float m_NeighbourValue;	// Cost it takes to move to this node, from the current node (either 1, or 1.414).

	sf::RectangleShape m_Shape;
public:
	// Constructor.
	Node() = default;	/* Default constructor. */
	Node(const sf::Vector2u &p_PixelPosition, const sf::Vector2u &p_Size, const sf::Vector2u &p_GraphArrayPosition, NodeState p_State = NodeState::OPEN);
	~Node();

	// Draw method.
	void draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_States) const;
	
	// Accessor methods.
	int getID() const;
	void setID(int p_ID);

	sf::Vector2u getPixelPosition() const;
	sf::Vector2u getGraphArrayPosition() const;
	sf::Vector2u getSize() const;

	bool getIsPath() const;
	/* Cannot set Path, as that's done through its NodeState, set its NodeState to anything but Wall or Base, to set it as a possible path. */

	NodeState getNodeState() const;
	void setNodeState(const NodeState &p_NodeState);

	float getGValue() const;
	void setGValue(float p_GValue);
	
	float getHValue() const;
	void setHValue(float p_HValue);

	float getFValue() const;
	void setFValue(float p_FValue);

	float getNeighbourValue() const;
	void setNeighbourValue(float p_NeighbourValue);

	// Compare nodes based on their m_FValue - sort by lowest f cost when using the less than operator to compare.
	// This is used in the Graph class, when we sort the open list, using the sort method.
	inline bool operator < (const Node &p_Other) const { 
		return this->m_FValue < p_Other.m_FValue; 
	}

	// Check for equality, using the nodes' unique ID.
	inline bool operator == (const Node &p_Other) const {
		//return this->m_PixelPosition == p_Other.m_PixelPosition;
		return this->m_ID == p_Other.m_ID;
	}
};