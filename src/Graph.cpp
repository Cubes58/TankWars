#include "Graph.h"

Graph::Graph() {
	for (int i = 0; i < s_m_ColumnSize; i++) {
		for (int j = 0; j < s_m_RowSize; j++) {
			Node *temp = new Node(sf::Vector2u(800 / (s_m_PixelSize * i), 600 / (s_m_PixelSize * j)), sf::Vector2u(20, 20));
		}
	}
}

Graph::~Graph() {
	for (auto i : m_Grid) {
		for (auto j : i) {
			if (j != nullptr) {
				delete j;
				j = nullptr;
			}
		}
	}
}

void Graph::draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_State) const {
	for (auto i : m_Grid) {
		for (auto j : i) {
			j->draw(p_RenderTarget, p_State);
		}
	}
}

NS::NodeState Graph::getNodeState(const sf::Vector2u &p_GridPosition) const {
	return m_Grid[p_GridPosition.x][p_GridPosition.y]->getNodeState();
}

void Graph::setNodeState(const sf::Vector2u &p_GridPosition, NS::NodeState p_NodeState) {
	m_Grid[p_GridPosition.x][p_GridPosition.y]->setNodeState(p_NodeState);
}