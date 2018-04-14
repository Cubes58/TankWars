#include "Node.h"

Node::Node(int p_ID, const sf::Vector2u &p_PixelPosition, const sf::Vector2u &p_Size, const sf::Vector2u &p_GraphArrayPosition, NodeState p_State)
	: m_ID(p_ID), m_PixelPosition(p_PixelPosition), m_Size(p_Size), m_State(p_State), m_GraphArrayPosition(p_GraphArrayPosition) {

	// Set some values, so the node can be drawn in debug mode.
	const float outLineThinkness(-0.5);
	m_Shape.setOrigin(sf::Vector2f((float)m_Size.x / 2, (float)m_Size.y / 2));
	m_Shape.setPosition(static_cast<sf::Vector2f>(m_PixelPosition));
	m_Shape.setSize(sf::Vector2f((float)m_Size.x, (float)m_Size.y));
	m_Shape.setOutlineThickness(outLineThinkness);
	m_Shape.setOutlineColor(sf::Color::Black);
	m_IsPath = false;
	setNodeState(m_State);		// Its state is set, but not the node colour, so set it here.
}

Node::~Node() {

}

void Node::draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_States) const {
	p_RenderTarget.draw(m_Shape);
}

int Node::getID() const {
	return m_ID;
}

sf::Vector2u Node::getPixelPosition() const {
	return m_PixelPosition;
}

sf::Vector2u Node::getGraphArrayPosition() const {
	return m_GraphArrayPosition;
}

sf::Vector2u Node::getSize() const {
	return m_Size;
}

bool Node::getIsPath() const {
	return m_IsPath;
}

NodeState Node::getNodeState() const {
	return m_State;
}

void Node::setNodeState(const NodeState &p_State) {
	if ((m_State == NodeState::BASE || m_State == NodeState::WALL) && (p_State == NodeState::GOAL || p_State == NodeState::START)) {
		return;
	}
	
	m_State = p_State;

	// Set the colour of the node, for debugging, depending on its set type.
	// Also, set whether it's a possible path or not, based on its NodeState.
	switch (m_State) {
	case NodeState::PATH:
		m_Shape.setFillColor(sf::Color(255, 0, 0, 120));		 // Red.
		m_IsPath = true;
		break;
	case NodeState::WALL:
		m_Shape.setFillColor(sf::Color(0, 0, 0, 120));			// Black.
		m_IsPath = false;
		break;
	case NodeState::GOAL:
		m_Shape.setFillColor(sf::Color(255, 255, 0, 120));		// Yellow.
		m_IsPath = true;
		break;
	case NodeState::START:
		m_Shape.setFillColor(sf::Color(0, 0, 255, 120));		// Blue.
		m_IsPath = true;
		break;
	case::NodeState::OPEN:
		m_Shape.setFillColor(sf::Color(132, 23, 255, 120));		// Purple.
		m_IsPath = true;
		break;
	case::NodeState::CLOSED:
		m_Shape.setFillColor(sf::Color(255, 110, 32, 120));		// Orange.
		m_IsPath = true;
		break;
	case::NodeState::CURRENT:
		m_Shape.setFillColor(sf::Color(255, 12, 225, 120));		// Magenta.
		m_IsPath = true;
		break;
	case::NodeState::BASE:
		m_Shape.setFillColor(sf::Color(45, 255, 239, 120));		// Cyan.
		m_IsPath = false;
		break;
	case::NodeState::NOTHING:
		m_Shape.setFillColor(sf::Color(0, 0, 0, 0));			// Clear.
		m_IsPath = true;
		break;
	default:
		m_Shape.setFillColor(sf::Color(255, 255, 255, 175));	// Default: White.
		m_IsPath = false;
		break;
	}
}

float Node::getGValue() const {
	return m_GValue;
}

void Node::setGValue(float p_GValue) {
	m_GValue = p_GValue;
}

float Node::getHValue() const {
	return m_HValue;
}

void Node::setHValue(float p_HValue) {
	m_HValue = p_HValue;
}

float Node::getFValue() const {
	return m_FValue;
}

void Node::setFValue(float p_FValue) {
	m_FValue = p_FValue;
}

float Node::getNeighbourValue() const {
	return m_NeighbourValue;
}

void Node::setNeighbourValue(float p_NeighbourValue) {
	m_NeighbourValue = p_NeighbourValue;
}