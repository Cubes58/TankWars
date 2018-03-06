#include "Node.h"

Node::Node(const sf::Vector2u &p_Position, const sf::Vector2u &p_Size) 
	: m_Position(p_Position), m_Size(p_Size) {

}

Node::~Node() {

}

void Node::draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_States) const {
	p_RenderTarget.draw(m_Shape);
}

int Node::calculateHeuristic(float p_ParentGScore, const sf::Vector2u &p_CurrentPosition, const sf::Vector2u &p_Goal) {
	int temp = 0;

	return temp;
}

sf::Vector2u Node::getPosition() {
	return m_Position;
}

void Node::setNodeState(NS::NodeState p_State) {
	m_State = p_State;
}

NS::NodeState Node::getNodeState() const {
	return m_State;
}