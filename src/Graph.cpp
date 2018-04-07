#include <map>

#include "Graph.h"

Graph::Graph() {
	int rowPositionOffset = 10;
	int columnPositionOffset = 10;
	int id = 0;

	for (int column = 0; column < s_m_ColumnSize; column++) {
		for (int row = 0; row < s_m_RowSize; row++) {
			Node *node;
			// If the node is on the edge of the map set it to a wall.
			if (column == 0 || column == s_m_ColumnSize - 1 || row == 0 || row == s_m_RowSize - 1 || row == s_m_RowSize - 2) {
				node = new Node(sf::Vector2u((column * s_m_PixelSize) + rowPositionOffset, (row * s_m_PixelSize) + columnPositionOffset),
					sf::Vector2u(s_m_PixelSize, s_m_PixelSize), sf::Vector2u(column, row), NodeState::WALL);
			}
			else {	/* Else set it to nothing, so it's clear. */
				node = new Node(sf::Vector2u((column * s_m_PixelSize) + rowPositionOffset, (row * s_m_PixelSize) + columnPositionOffset),
					sf::Vector2u(s_m_PixelSize, s_m_PixelSize), sf::Vector2u(column, row), NodeState::NOTHING);
			}
			node->setID(id++);
			m_Nodes[row][column] = node;
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

bool Graph::aStarSearchAlgorithm(Node &p_StartNode, Node &p_GoalNode, std::list<Node*> &p_Path) {	
	// Set the start and goal Node states, so they can be seen on debug mode.
	p_StartNode.setNodeState(NodeState::START);
	p_GoalNode.setNodeState(NodeState::GOAL);
	
	std::list<Node> openList;		// List of nodes that haven't been explored.
	std::list<Node> closedList;		// List of nodes that have been explored.

	std::map<int, Node> cameFrom;

	// Make sure the closed list is empty.
	closedList.empty();

	// Set the starting node.
	Node currentNode = p_StartNode;
	currentNode.setGValue(0);
	currentNode.setFValue(currentNode.getGValue() + currentNode.calculateManhattanHeuristic(currentNode, p_GoalNode));
	
	// Add the starting node to the list.
	openList.push_back(currentNode);

	// While there are nodes in the open list.
	while (!openList.empty()) {
		openList.sort();		// Sort the open list, according to the node's m_FValue.

		// Set the node to the one with the best (lowest) m_FValue.
		currentNode = openList.front();
		currentNode.setNodeState(NodeState::CLOSED);
		closedList.push_back(currentNode);	// Add it to the closed list.
		openList.pop_front();

		if (currentNode == p_GoalNode) {
			p_Path = constructPath(p_GoalNode, cameFrom);
			return true;		// Path found.
		}		

		Node *bestNeighbourNode = &currentNode;
		for (auto &neighbourNode : getNeighbours(currentNode)) {
			float newGCost = currentNode.getGValue() + neighbourNode->getNeighbourValue();											

			if (neighbourNode->getNodeState() == NodeState::CLOSED)
				continue;

			bool inOpenList = false;
			for (auto &i : openList) {
				if (*neighbourNode == i) {
					inOpenList = true;
				}
			}

			neighbourNode->calculateManhattanHeuristic(*neighbourNode, p_GoalNode);
			neighbourNode->setGValue(newGCost);
			neighbourNode->setFValue(newGCost + neighbourNode->getHValue());

			if (!inOpenList)
				openList.push_back(*neighbourNode);

			float best = currentNode.getFValue();
			if (best < neighbourNode->getFValue() && bestNeighbourNode->getFValue() < neighbourNode->getFValue()) {
				continue;
			}			
		}
		cameFrom[bestNeighbourNode->getID()] = currentNode;
	}

	return false;		// A path wasn't found.
}

std::vector<Node*> Graph::getNeighbours(Node &p_Node) {
	static const float diagonalCost(1.414);
	static const float normalCost(1);

	std::vector<Node*> neighbours;
	for (int column = p_Node.getGraphArrayPosition().x - 1; column != p_Node.getGraphArrayPosition().x + 2; column++) {
		for (int row = p_Node.getGraphArrayPosition().y - 1; row != p_Node.getGraphArrayPosition().y + 2; row++) {
			if (p_Node.getGraphArrayPosition().x == column && p_Node.getGraphArrayPosition().y == row)
				continue;	// Don't want to add the current node to the neighbours.

			// Ensure the node is within the array's bounds.
			if (column >= 0 && column <= s_m_ColumnSize && row >= 0 && row <= s_m_RowSize) {
				Node* neighbourNode = &getNode(sf::Vector2u(column, row));
				// Check whether the node is a path node - can be moved along.
				if (neighbourNode->getIsPath()) {
					// Check whether the node is diagonal from the current node, or not, as if it is then use a different (diagonal cost).
					if ((column == p_Node.getGraphArrayPosition().x - 1 || column == p_Node.getGraphArrayPosition().x + 1) && 
						(row == p_Node.getGraphArrayPosition().y - 1 || row == p_Node.getGraphArrayPosition().y + 1)) {
						neighbourNode->setNeighbourValue(diagonalCost);
					}
					else
						neighbourNode->setNeighbourValue(normalCost);
					neighbours.push_back(neighbourNode);			// Add the neighbour node to the vector of neighbours.
				}
			}
		}
	}

	// Return the neighbours.
	return neighbours;
}

std::list<Node*> Graph::constructPath(Node &p_GoalNode, std::map<int, Node> cameFrom) {
	std::list<Node*> path;
	
	path.push_back(&p_GoalNode); 
	for (auto &i : cameFrom) {		
		Node* pathNode = &getNode(i.first);
		if (!(pathNode->getNodeState() == NodeState::START || pathNode->getNodeState() == NodeState::GOAL))
			pathNode->setNodeState(NodeState::PATH);
		
		path.push_back(pathNode);
	}	

	return path;
}

void Graph::clearNodes() {
	for (auto &i : m_Nodes) {
		for (auto &j : i) {
			j->setNodeState(NodeState::NOTHING);
		}
	}
}

Node &Graph::getPixelNode(const sf::Vector2u &p_NodePixelPosition) {
	for (auto &i : m_Nodes) {
		for (auto &j : i) {
			if ((j->getPosition().x - (j->getSize().x / 2) <= p_NodePixelPosition.x && j->getPosition().x + (j->getSize().x / 2) >= p_NodePixelPosition.x)
				&& (j->getPosition().y - (j->getSize().y / 2) <= p_NodePixelPosition.y && j->getPosition().y + (j->getSize().y / 2) >= p_NodePixelPosition.y)) {
				return *j;
			}
		}
	}
}

Node &Graph::getNode(const sf::Vector2u &p_NodeGraphPosition) {
	return *m_Nodes[p_NodeGraphPosition.y][p_NodeGraphPosition.x];
}

Node &Graph::getNode(int id) const {
	for (auto & i : m_Nodes) {
		for (auto & j : i) {
			if (j->getID() == id)
				return *j;
		}
	}
}

NodeState Graph::getNodeState(const sf::Vector2u &p_GridPosition) const {
	return m_Nodes[p_GridPosition.y][p_GridPosition.x]->getNodeState();
}

void Graph::setNodeState(const sf::Vector2u &p_GridPosition, NodeState p_NodeState) {
	m_Nodes[p_GridPosition.y][p_GridPosition.x]->setNodeState(p_NodeState);
}