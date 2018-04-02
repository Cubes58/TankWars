#include "Node.h"

Node::Node(const sf::Vector2u &p_Position, const sf::Vector2u &p_Size, const sf::Vector2u &p_GraphArrayPosition, NodeState p_State)
	: m_Position(p_Position), m_Size(p_Size), m_State(p_State), m_GraphArrayPosition(p_GraphArrayPosition) {

	// Set some values, so the node can be drawn in debug mode.
	m_Shape.setOrigin(sf::Vector2f(m_Size.x / 2, m_Size.y / 2));
	m_Shape.setPosition(static_cast<sf::Vector2f>(m_Position));
	m_Shape.setSize(sf::Vector2f(m_Size.x, m_Size.y));
	setNodeState(m_State);		// Node colour set here.
}

Node::~Node() {

}

void Node::draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_States) const {
	p_RenderTarget.draw(m_Shape);
}

// The G cost is the cost value of how far that node is from the starting node.		// Diagonal moves tend to cost more than vertical/horizontal ones
// The H cost is how far away the node is from the end node - can also modifiy this to punish certain terrain.
// The F cost is the total of the two added together (G + H).

int Node::calculateHeuristic(float p_ParentGScore, const sf::Vector2u &p_TankPosition, const sf::Vector2u &p_GoalPosition) {
	int temp = 0;

	return temp;
}

sf::Vector2u Node::getPosition() {
	return m_Position;
}

void Node::setNodeState(NodeState p_State) {
	m_State = p_State;

	// Set the colour of the node, for debugging, depending on its set type.
	switch (m_State) {
	case NodeState::PATH:
		m_Shape.setFillColor(sf::Color(255, 110, 0, 150));		// Orange.
		break;
	case NodeState::WALL:
		m_Shape.setFillColor(sf::Color(0, 0, 0, 150));			// Black.
		break;
	case NodeState::GOAL:
		m_Shape.setFillColor(sf::Color(255, 255, 0, 120));		// Yellow.
		break;
	case NodeState::START:
		m_Shape.setFillColor(sf::Color(0, 0, 255, 120));		// Blue.
		break;
	case::NodeState::OPEN:
		m_Shape.setFillColor(sf::Color(255, 0, 0, 50));			// Red.
		break;
	case::NodeState::CLOSED:
		m_Shape.setFillColor(sf::Color(0, 255, 0, 70));			// Green.
		break;
	case::NodeState::CURRENT:
		m_Shape.setFillColor(sf::Color(255, 12, 225, 150));		// Magenta.
		break;
	default:
		m_Shape.setFillColor(sf::Color(255, 255, 255, 150));	// Default: White.
		break;
	}
}

NodeState Node::getNodeState() const {
	return m_State;
}