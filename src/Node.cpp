#include "Node.h"

Node::Node(const sf::Vector2u &p_Position, const sf::Vector2u &p_Size, const sf::Vector2u &p_GraphArrayPosition, NodeState p_State)
	: m_Position(p_Position), m_Size(p_Size), m_State(p_State), m_GraphArrayPosition(p_GraphArrayPosition) {

	const float outLineThinkness(-0.5);
	// Set some values, so the node can be drawn in debug mode.
	m_Shape.setOrigin(sf::Vector2f((float)m_Size.x / 2, (float)m_Size.y / 2));
	m_Shape.setPosition(static_cast<sf::Vector2f>(m_Position));
	m_Shape.setSize(sf::Vector2f((float)m_Size.x, (float)m_Size.y));
	m_Shape.setOutlineThickness(outLineThinkness);
	m_Shape.setOutlineColor(sf::Color::Black);
	setNodeState(m_State);		// Node colour set here.
}

Node::~Node() {

}

void Node::draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_States) const {
	p_RenderTarget.draw(m_Shape);
}

int Node::calculateManhattanHeuristic(Node &p_PreviousNode, Node &p_GoalNode) {
	int xDistance = p_PreviousNode.getPosition().x - p_GoalNode.getPosition().x;
	int yDistance = p_PreviousNode.getPosition().y - p_GoalNode.getPosition().y;

	/* NOTE: CHANGE TO USE ARRAY POSITION INSTEAD OF PIXEL POS!!! */

	return std::abs(xDistance) + std::abs(yDistance);
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
	case::NodeState::BASE:
		m_Shape.setFillColor(sf::Color(45, 255, 239, 160));		// Cyan.
		break;
	default:
		m_Shape.setFillColor(sf::Color(255, 255, 255, 150));	// Default: White.
		break;
	}
}

NodeState Node::getNodeState() const {
	return m_State;
}

void Node::setParentNodeGraphArrayPosition(const sf::Vector2u &p_ParentNodeGraphArrayPosition) {
	m_ParentNodeGraphArrayPosition = p_ParentNodeGraphArrayPosition;
}

sf::Vector2u Node::getParentNodeGraphArrayPosition() {
	return m_ParentNodeGraphArrayPosition;
}

sf::Vector2u Node::getGraphArrayPosition() {
	return m_GraphArrayPosition;
}

void Node::setGValue(float p_GValue) {
	m_GValue = p_GValue;
}

float Node::getGValue() {
	return m_GValue;
}

void Node::setHValue(float p_HValue) {
	m_HValue = p_HValue;
}

float Node::getHValue() {
	return m_HValue;
}

void Node::setFValue(float p_FValue) {
	m_FValue = p_FValue;
}

float Node::getFValue() {
	return m_FValue;
}