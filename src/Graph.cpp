#include <map>

#include "Graph.h"

Graph::Graph() {
	int rowPositionOffset = 10;
	int columnPositionOffset = 10;
	int id = 0;

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
					sf::Vector2u(s_m_PixelSize, s_m_PixelSize), sf::Vector2u(column, row), NodeState::NOTHING);
			}
			id++;
			temp->setId(id);
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

bool Graph::aStarSearchAlgorithm(Node &p_StartNode, Node &p_GoalNode, std::list<Node*> &p_Path) {	
	std::list<Node> openList;		// List of nodes that haven't been explored.
	std::list<Node> closedList;		// List of nodes that have been explored.

	std::map<int, Node> cameFrom;

	// Make sure the closed list is empty.
	closedList.empty();

	// Set the starting node.
	Node currentNode = p_StartNode;
	p_StartNode.setNodeState(NodeState::START);

	currentNode.setParentNode(currentNode.getGraphArrayPosition());		// Set it to itself.
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
				
		std::cout << "Cur: " << currentNode.getId() << std::endl;

		if (currentNode == p_GoalNode) {
			p_Path = constructPath(p_GoalNode, cameFrom);
			return true;		// Path found.
		}		

		for (auto &neighbourNode : getNeighbours(currentNode)) {
			float newGCost = currentNode.getGValue() + neighbourNode->getNeighbourVal();											

			if (neighbourNode->getNodeState() == NodeState::CLOSED)
				continue;

			bool temp = false;

			for (auto &i : openList) {
				if (*neighbourNode == i) {
					temp = true;
				}
			}

			neighbourNode->calculateManhattanHeuristic(*neighbourNode, p_GoalNode);


			neighbourNode->setGValue(newGCost);
			neighbourNode->setFValue(newGCost + neighbourNode->getHValue());

			if (temp == false)
				openList.push_back(*neighbourNode);

			std::cout << "Nei: " << neighbourNode->getId() << std::endl;

			float best = currentNode.getFValue();

			if (neighbourNode->getFValue() >= best)
				continue;

			cameFrom[neighbourNode->getId()] = currentNode;				
		}
	}

	return false;		// A path wasn't found.
}

std::vector<Node*> Graph::getNeighbours(Node &p_Node) {
	static const float diagonalCost(1.414);
	static const float normalCost(1);

	std::vector<Node*> neighbours;
	std::vector<bool> diagonals;

	for (int column = p_Node.getGraphArrayPosition().x - 1; column != p_Node.getGraphArrayPosition().x + 2; column++) {
		for (int row = p_Node.getGraphArrayPosition().y - 1; row != p_Node.getGraphArrayPosition().y + 2; row++) {
			if (p_Node.getGraphArrayPosition().x == column && p_Node.getGraphArrayPosition().y == row)
				continue;

			if (column >= 0 && column <= s_m_ColumnSize && row >= 0 && row <= s_m_RowSize) {
				Node* temp = &getNode(sf::Vector2u(column, row));
				// Check whether the node is a path node.
				if (temp->getIsPath()) {
					neighbours.push_back(temp);

					if (column == p_Node.getGraphArrayPosition().x - 1 || column == p_Node.getGraphArrayPosition().x + 1) {
						if (row == p_Node.getGraphArrayPosition().y - 1 || row == p_Node.getGraphArrayPosition().y + 1)
							diagonals.push_back(true);
						else
							diagonals.push_back(false);
					}
					else
						diagonals.push_back(false);
				}
			}
		}
	}

	for (int i = 0; i < neighbours.size(); i++) {
		if (diagonals[i])
			neighbours[i]->setNeighbourVal(diagonalCost);
		else 
			neighbours[i]->setNeighbourVal(normalCost);
		neighbours[i]->setParentNode(sf::Vector2u(p_Node.getGraphArrayPosition().x, p_Node.getGraphArrayPosition().y));
	}

	return neighbours;
}

std::list<Node*> Graph::constructPath(Node &p_GoalNode, std::map<int, Node> cameFrom) {
	std::list<Node*> path;
	
	path.push_back(&p_GoalNode);

	for (auto & i : cameFrom) {
		//Node temp = i.second;
		//i.second.setNodeState(NodeState::PATH);
		//temp.setNodeState(NodeState::PATH);
		
		Node* temp = &getNode(i.first);
		temp->setNodeState(NodeState::PATH);
		
		path.push_back(temp);
	}

	//Node *currentNode = &p_GoalNode;
	//path.push_back(currentNode);
	//p_ClosedList.pop_back();
	//
	//
	//int counter = 0;
	//for (std::list<Node>::reverse_iterator node = p_ClosedList.rbegin(); node != p_ClosedList.rend(); ++node) {		
	//	path.push_front(&*node);
	//	node->setNodeState(NodeState::PATH);
	//	std::cout << "Node: " << std::endl;
	//	std::cout << "PosX: " << node->getPosition().x << std::endl;
	//	std::cout << "PosY: " << node->getPosition().y << std::endl;
	//	std::cout << "Node counter: " << ++counter << std::endl;
	//}
	//Node *currentNode = &p_GoalNode;
	//path.push_back(currentNode);
	//// While a parent exists - if it is its own parent then you're there!
	//while (currentNode->getParentNode() != currentNode->getGraphArrayPosition()) {
	//	currentNode = &getNode(currentNode->getParentNode());
	//	path.push_back(&*currentNode);
	//	currentNode->setNodeState(NodeState::PATH);
	//}*/
	

	return path;
}

void Graph::clearNodes() {
	//for (auto &i : m_Nodes) {
	//	for (auto &j : i) {
	//		j->setNodeState(NodeState::NOTHING);
	//	}
	//}
}

Node *Graph::getPixelNode(const sf::Vector2u &p_NodePixelPosition) {
	for (auto &i : m_Nodes) {
		for (auto &j : i) {
			if ((j->getPosition().x - 10 <= p_NodePixelPosition.x && j->getPosition().x + 10 >= p_NodePixelPosition.x)
				&& (j->getPosition().y - 10 <= p_NodePixelPosition.y && j->getPosition().y + 10 >= p_NodePixelPosition.y)) {
				return j;
			}
		}
	}
	
	return nullptr;
}

Node &Graph::getNode(const sf::Vector2u &p_NodeGraphPosition) {
	return *m_Nodes[p_NodeGraphPosition.y][p_NodeGraphPosition.x];
}

Node & Graph::getNode(int id) const {
	for (auto & i : m_Nodes) {
		for (auto & j : i) {
			if (j->getId() == id)
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