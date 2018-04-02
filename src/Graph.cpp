#include "Graph.h"

Graph::Graph() {
	int rowPositionOffset = 10;
	int columnPositionOffset = 10;

	for (int column = 0; column < s_m_ColumnSize; column++) {
		for (int row = 0; row < s_m_RowSize; row++) {
			Node *temp;
			// If the node is on the edge of the map set it to a wall.
			if (column == 0 || column == s_m_ColumnSize - 1 || row == 0 || row == s_m_RowSize - 1 || row == s_m_RowSize - 2) {
				temp = new Node(sf::Vector2u((column * s_m_PixelSize) + rowPositionOffset, (row * s_m_PixelSize) + columnPositionOffset),
					sf::Vector2u(s_m_PixelSize, s_m_PixelSize), sf::Vector2u(column, row), NodeState::WALL);
			}
			else {	/* Else set it to an open node. */
				temp = new Node(sf::Vector2u((column * s_m_PixelSize) + rowPositionOffset, (row * s_m_PixelSize) + columnPositionOffset),
					sf::Vector2u(s_m_PixelSize, s_m_PixelSize), sf::Vector2u(column, row), NodeState::OPEN);
			}
			m_Grid[row][column] = temp;
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
	for (auto &i : m_Grid) {
		for (auto &j : i) {
			j->draw(p_RenderTarget, p_State);
		}
	}
}

void Graph::aStarSearchAlgorithm(Node p_CurrentNode, Node p_GoalNode, std::list<Node> p_Path) {
	std::list<Node> openList;		// List of nodes that haven't been explored.
	std::list<Node> closedList;		// List of nodes that have been explored.

	// Empty the closed list.
	closedList.empty();


}

NodeState Graph::getNodeState(const sf::Vector2u &p_GridPosition) const {
	return m_Grid[p_GridPosition.x][p_GridPosition.y]->getNodeState();
}

void Graph::setNodeState(const sf::Vector2u &p_GridPosition, NodeState p_NodeState) {
	m_Grid[p_GridPosition.x][p_GridPosition.y]->setNodeState(p_NodeState);
}