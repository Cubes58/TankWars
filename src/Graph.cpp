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
			m_Nodes[row][column] = temp;
		}
	}
}

Graph::~Graph() {
	for (auto &i : m_Nodes) {
		for (auto &j : i) {
			if (j != nullptr) {
				delete j;
				j = nullptr;
			}
		}
	}
}

void Graph::draw(sf::RenderTarget &p_RenderTarget, sf::RenderStates p_State) const {
	for (auto &i : m_Nodes) {
		for (auto &j : i) {
			j->draw(p_RenderTarget, p_State);
		}
	}
}

bool Graph::aStarSearchAlgorithm(Node &p_StartNode, Node &p_GoalNode, std::list<Node> &p_Path) {
	std::list<Node> openList;		// List of nodes that haven't been explored.
	std::list<Node> closedList;		// List of nodes that have been explored.

	// Make sure the closed list is empty.
	closedList.empty();

	// Set the starting node.
	Node currentNode = p_StartNode;
	currentNode.setParentNodeGraphArrayPosition(currentNode.getGraphArrayPosition());		// Set it to itself.
	currentNode.setGValue(0);
	currentNode.setFValue(currentNode.getGValue() + currentNode.calculateManhattanHeuristic(currentNode, p_GoalNode));

	// Add the starting node to the list.
	openList.push_back(currentNode);

	// While there are nodes in the open list.
	while (!openList.empty()) {
		openList.sort();		// Sort the open list, according to the node's m_FValue.

		if (currentNode == p_GoalNode) {
			p_Path = constructPath(p_GoalNode);
			return true;		// Path found.
		}

		// Set the node to the one with the best (lowest) m_FValue.
		currentNode = openList.front();
		openList.pop_front();				// Remove the node and the front/update the list.
		closedList.push_back(currentNode);	// Add it to the closed list.

		for (auto &neighbour : getNeighbours(currentNode)) {
			bool inClosedList(false);

			// Iterate through the closed list, looking for the neighbour.
			for (auto &closedListNode : closedList) {
				if (*neighbour == closedListNode)	// IF THIS DOESN'T WORK CHECK THE GRAPH ARRAY POSITION.
					inClosedList = true;
			}

			if (!inClosedList) {
				openList.push_back(*neighbour);
				neighbour->setNodeState(NodeState::CLOSED);
			}
		}
	}

	return false;		// A path wasn't found.
}

std::vector<Node*> Graph::getNeighbours(Node &p_Node) {
	static const float diagonalCost(1.414);
	static const float normalCost(1);

	std::vector<Node*> neighbours;
	std::vector<bool> diagonals;


	return neighbours;
}

std::list<Node> Graph::constructPath(Node &p_GoalNode) {
	std::list<Node> path;
	path.empty();

	Node currentNode = p_GoalNode;
	path.push_back(currentNode);

	// While a parent exists.
	while (currentNode.getParentNodeGraphArrayPosition() != currentNode.getGraphArrayPosition()) {
		currentNode = getNode(currentNode.getParentNodeGraphArrayPosition());
		path.push_back(currentNode);
	}

	return path;
}

Node *Graph::getPixelNode(const sf::Vector2u &p_NodePixelPosition) {
	// Node x every 10 pixels.
	// Node y every 10 pixels.

	/*
	sf::Vector2u tankPosition = p_NodePixelPosition;

	int xRe = tankPosition.x % 10;
	int yRe = tankPosition.y % 10;

	int xPos = tankPosition.x / 10;
	int yPos = tankPosition.y / 10;

	int xFinalPos = xPos - xRe;
	int yFinalPos = yPos - yRe;
	*/

	for (auto &i : m_Nodes) {
		for (auto &j : i) {
			// Make sure it's in the correct row/column.
			if ((j->getPosition().x - 10 >= p_NodePixelPosition.x && j->getPosition().x + 10 <= p_NodePixelPosition.x)
				&& (j->getPosition().y - 10 >= p_NodePixelPosition.y && j->getPosition().y + 10 <= p_NodePixelPosition.y)) {
				return j;
			}
		}
	}
	
	return nullptr;
}

Node &Graph::getNode(const sf::Vector2u &p_NodeGraphPosition) {
	return *m_Nodes[p_NodeGraphPosition.x][p_NodeGraphPosition.y];
}

NodeState Graph::getNodeState(const sf::Vector2u &p_GridPosition) const {
	return m_Nodes[p_GridPosition.x][p_GridPosition.y]->getNodeState();
}

void Graph::setNodeState(const sf::Vector2u &p_GridPosition, NodeState p_NodeState) {
	m_Nodes[p_GridPosition.x][p_GridPosition.y]->setNodeState(p_NodeState);
}