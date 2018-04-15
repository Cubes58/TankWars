#include "Graph.h"

#include <iostream>

Graph::Graph() {
	int rowPositionOffset = (s_m_PixelSize / 2);		// Distance between the nodes, in the row.
	int columnPositionOffset = (s_m_PixelSize / 2);		// Distance between the nodes, in the column.
	int nodeID = 0;		// Unique ID given to each node.

	for (int column = 0; column < s_m_ColumnSize; column++) {
		for (int row = 0; row < s_m_RowSize; row++) {
			Node *node;
			// If the node is on the edge of the map set it to a wall.
			if (column == 0 || column == s_m_ColumnSize - 1 || row == 0 || row == s_m_RowSize - 1 || row == s_m_RowSize - 2) {
				node = new Node(nodeID++, sf::Vector2u((column * s_m_PixelSize) + rowPositionOffset, (row * s_m_PixelSize) + columnPositionOffset),
					sf::Vector2u(s_m_PixelSize, s_m_PixelSize), sf::Vector2u(column, row), NodeState::WALL);
			}
			else {	/* Else set it to nothing, so it's clear. */
				node = new Node(nodeID++, sf::Vector2u((column * s_m_PixelSize) + rowPositionOffset, (row * s_m_PixelSize) + columnPositionOffset),
					sf::Vector2u(s_m_PixelSize, s_m_PixelSize), sf::Vector2u(column, row), NodeState::NOTHING);
			}
			m_Nodes[row][column] = node;
		}
	}

	// Expect a max of 20 bases, on the map.
	m_BasesFound.reserve(20);
}

Graph::~Graph() {
	// Manually clean up the memory, as we're not using "smart" pointers.
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
	// Get every node from m_Nodes, and call the draw method, which draws the sf::RectangleShape m_Shape the node has.
	for (auto &i : m_Nodes) {
		for (auto &j : i) {
			j->draw(p_RenderTarget, p_State);
		}
	}
}

bool Graph::aStarSearchAlgorithm(Node *p_StartNode, Node *p_GoalNode, std::list<Node*> &p_Path) {	
	// Reset the path, readying it for the new one.
	p_Path.clear();
	clearNodes();

	// If the node, given as a goal node is either a base or a wall node, we cannot move to it, so we need to calculate the closest node that's a path.
	if (p_GoalNode->getNodeState() == NodeState::BASE) {
		// Sets the goal node, to the closest node that's a path (i.e. not a base or wall).
		p_GoalNode = calculateClosestPathNode(p_GoalNode);
	}
	if (p_StartNode->getNodeState() == NodeState::BASE) {
 		p_StartNode = calculateClosestPathNode(p_StartNode);
	}
	
	std::list<Node> openList;		// List of nodes that haven't been explored.
	std::list<Node> closedList;		// List of nodes that have been explored.

	// Set the starting node and its values (G, H, and F).
	Node currentNode = *p_StartNode;
	currentNode.setGValue(0);
	currentNode.setFValue(currentNode.getGValue() + calculateManhattanHeuristic(currentNode, *p_GoalNode));
	
	// Add the starting node to the list.
	openList.push_back(currentNode);

	// While there are nodes in the open list, to check for a path with. Do the following:
	while (!openList.empty()) {
		openList.sort();		// Sort the open list, according to the node's m_FValue (overloaded the < operator, which is what it uses).

		// Set the node to the one with the best (lowest) m_FValue - now the openList has been sorted by m_FValue it's the one on the front.
		currentNode = openList.front();
		currentNode.setNodeState(NodeState::CLOSED);
		closedList.push_back(currentNode);	// Add it to the closed list.
		openList.pop_front();				// Remove it from the open list.

		Node *bestNeighbourNode = &currentNode;
		if (currentNode == *p_GoalNode) {
			p_GoalNode->setParent(getNode(bestNeighbourNode->getID()).getParent());
			p_Path = constructPath(*p_GoalNode);
			return true;		// Path found.
		}		

		// Keep track of the best neighbour, so we can compare the others, and update it, if there's one better.
		for (auto &neighbourNode : getNeighbours(currentNode)) {
			// The new G cost, of the neighbourNode is the current node G cost, plus the neighbour move cost (1, or 1.414).  
			float newGCost = currentNode.getGValue() + neighbourNode->getNeighbourValue();											

			// If the neighbour is on the closed list, get the next neighbour.
			if (neighbourNode->getNodeState() == NodeState::CLOSED || !neighbourNode->getIsPath())
				continue;

			// Now we know the node isn't on the closed list,
			// Check whether it's on the open list.
			bool inOpenList = false;
			for (auto &i : openList) {
				if (*neighbourNode == i) {
					inOpenList = true;		/* Iterate through the open list, if found on the openList set inOpenList to true. */
				}
			}

			// Calculate an estimate distance between the neighbourNode, and the p_GoalNode, using the Manhattan heuristic.
			calculateManhattanHeuristic(*neighbourNode, *p_GoalNode);
			neighbourNode->setGValue(newGCost);									// Update the neighbour's g cost.
			neighbourNode->setFValue(newGCost + neighbourNode->getHValue());	// Update the neighbour's f cost.

			if (!inOpenList)
				openList.push_back(*neighbourNode);		/* If the neighbour isn't on the closed list, add it on. */
			else 
				bestNeighbourNode = neighbourNode;

			// Set the best cost to goal node, as the one we're on.
			// Node's that have a better score need to be kept track of.
			float best = currentNode.getFValue();
			if (best < neighbourNode->getFValue()) {// && bestNeighbourNode->getFValue() < neighbourNode->getFValue()) {
				continue;
			}	
		}
		// Set the best neighbour's parent to the current node.
		getNode(bestNeighbourNode->getID()).setParent(&getNode(currentNode.getID()));

		// DEBUG (visual) purposes: To see the open/closed list, in debug mode.
		for (auto &i : closedList) {
			if (!(i.getNodeState() == NodeState::START || i.getNodeState() == NodeState::GOAL))
				getNode(sf::Vector2u(i.getGraphArrayPosition())).setNodeState(NodeState::CLOSED);
		}
		for (auto &i : openList) {
			if (!(i.getNodeState() == NodeState::START || i.getNodeState() == NodeState::GOAL))
				getNode(sf::Vector2u(i.getGraphArrayPosition())).setNodeState(NodeState::OPEN);
		}
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
				continue;	// Don't want to add the current node to the neighbours, so get the next neighbour.

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

					Node *temp = &getNode(neighbourNode->getGraphArrayPosition());
					if (temp->getParent() == nullptr)
						temp->setParent(&getNode(p_Node.getGraphArrayPosition()));
				}
			}
		}
	}

	// Return the neighbours.
	return neighbours;
}

std::list<Node*> Graph::constructPath(Node &p_GoalNode) {
	std::list<Node*> path;		// Path found, and returned.

	// Add the goal node.
	path.push_back(&p_GoalNode);
	Node *node = p_GoalNode.getParent();
	while (node != nullptr && node->getParent()->getGraphArrayPosition() != node->getGraphArrayPosition()) {
		// Make sure there isn't an "infinite" loop. 
		if (node->getNodeState() == NodeState::PATH || node->getParent()->getNodeState() == NodeState::PATH) {
			break;		// Break out of the loop, and return whatever path it has. (Shouldn't ever happen.)
		}

		node->setNodeState(NodeState::PATH);		// Set the state, for debugging.
		path.push_back(node);						// Add the node, to the path.

		node = node->getParent();					// Set the node to its parent... to repeat!

		// If the parent is equal to itself, then we've reached the end of the path.
		// I.e. we've back tracked to the start node, from the goal node, using the parents, creating a path.
		// So, add the start node. This should be the final time time it iterates through this loop.
		if (node->getParent()->getGraphArrayPosition() == node->getGraphArrayPosition()) {
			path.push_back(node);
		}
	}

	// Make sure the back, and front of path are the goal/start nodes - for debugging.
	if (path.back()->getIsPath())
		path.back()->setNodeState(NodeState::START);
	if (path.front()->getIsPath())
		path.front()->setNodeState(NodeState::GOAL);

	return path;	// Return the path, to be used.
}

float Graph::calculateManhattanHeuristic(Node &p_CurrentNode, Node &p_GoalNode) {
	int xDistance = p_CurrentNode.getGraphArrayPosition().x - p_GoalNode.getGraphArrayPosition().x;
	int yDistance = p_CurrentNode.getGraphArrayPosition().y - p_GoalNode.getGraphArrayPosition().y;

	// Also want this value to be set for the node, so we can use it within the A star algorithm (getHValue()).
	// std::abs ensure we're always using the absolute value.
	p_CurrentNode.setHValue((float)(std::abs(xDistance) + std::abs(yDistance)) * 1.4);
	return p_CurrentNode.getHValue();
}

Node *Graph::calculateClosestPathNode(Node *p_Node) {
	p_Node->setNodeState(NodeState::BASE);
	Node tempNode = *p_Node;

	/* SIDE NODE: up and down are inverted - Nodes are created from the top of the screen, so +y = -y and -y = +y */
	int numberOfLeftBases(0);
	int numberOfRightBases(0);
	int numberOfUpBases(0);
	int numberOfDownBases(0);

	// Check how many base nodes are in each vertical/horizontal direction.
	Node *currentNode = &tempNode;
	// Left counter.
	while (!currentNode->getIsPath()) {
		numberOfLeftBases++;
		currentNode = &getNode(sf::Vector2u(currentNode->getGraphArrayPosition().x - 1, currentNode->getGraphArrayPosition().y));

		sf::Vector2u newPosition = currentNode->getGraphArrayPosition();
		if ((newPosition.x <= 0 || newPosition.x >= s_m_ColumnSize - 1) || newPosition.y <= 0 || newPosition.y >= s_m_RowSize - 1) {
			numberOfLeftBases = 5000; // Something so high it won't every be used!
			break;
		}
	}

	// Reset the tempGoalNode.
	tempNode = *p_Node;
	currentNode = &tempNode;
	// Right counter.
	while (!currentNode->getIsPath()) {
		numberOfRightBases++;
		currentNode = &getNode(sf::Vector2u(currentNode->getGraphArrayPosition().x + 1, currentNode->getGraphArrayPosition().y));

		sf::Vector2u newPosition = currentNode->getGraphArrayPosition();
		if ((newPosition.x <= 0 || newPosition.x >= s_m_ColumnSize - 1) || newPosition.y <= 0 || newPosition.y >= s_m_RowSize - 1) {
			numberOfRightBases = 5000; // Something so high it won't every be used!
			break;
		}
	}

	// Reset the tempGoalNode.
	tempNode = *p_Node;
	currentNode = &tempNode;
	// Up counter.
	while (!currentNode->getIsPath()) {
		numberOfUpBases++;
		currentNode = &getNode(sf::Vector2u(currentNode->getGraphArrayPosition().x, currentNode->getGraphArrayPosition().y + 1));

		sf::Vector2u newPosition = currentNode->getGraphArrayPosition();
		if ((newPosition.x <= 0 || newPosition.x >= s_m_ColumnSize - 1) || newPosition.y <= 0 || newPosition.y >= s_m_RowSize - 1) {
			numberOfUpBases = 5000; // Something so high it won't every be used!
			break;
		}
	}

	// Reset the tempGoalNode.
	tempNode = *p_Node;
	currentNode = &tempNode;
	// Down counter.
	while (!currentNode->getIsPath()) {
		numberOfDownBases++;
		currentNode = &getNode(sf::Vector2u(currentNode->getGraphArrayPosition().x, currentNode->getGraphArrayPosition().y - 1));

		sf::Vector2u newPosition = currentNode->getGraphArrayPosition();
		if ((newPosition.x <= 0 || newPosition.x >= s_m_ColumnSize - 1) || newPosition.y <= 0 || newPosition.y >= s_m_RowSize - 1) {
			numberOfDownBases = 5000; // Something so high it won't every be used!
			break;
		}
	}

	static int maxNumberOFNextBases(50);
	if (numberOfLeftBases <= numberOfRightBases && numberOfLeftBases <= numberOfUpBases && numberOfLeftBases <= numberOfDownBases && maxNumberOFNextBases >= numberOfLeftBases) {
		// Go Left of the goal node.
		getNode(sf::Vector2u(p_Node->getGraphArrayPosition().x, p_Node->getGraphArrayPosition().y - numberOfDownBases)).setNodeState(NodeState::NOTHING);
		p_Node = &getNode(sf::Vector2u(p_Node->getGraphArrayPosition().x - numberOfLeftBases, p_Node->getGraphArrayPosition().y));
	}
	else if (numberOfRightBases <= numberOfLeftBases && numberOfRightBases <= numberOfUpBases && numberOfRightBases <= numberOfDownBases && maxNumberOFNextBases >= numberOfRightBases) {
		// Go right of the goal node.
		getNode(sf::Vector2u(p_Node->getGraphArrayPosition().x, p_Node->getGraphArrayPosition().y - numberOfDownBases)).setNodeState(NodeState::NOTHING);
		p_Node = &getNode(sf::Vector2u(p_Node->getGraphArrayPosition().x + numberOfRightBases, p_Node->getGraphArrayPosition().y));
	}
	else if (numberOfUpBases <= numberOfLeftBases && numberOfUpBases <= numberOfRightBases && numberOfUpBases <= numberOfDownBases && maxNumberOFNextBases >= numberOfUpBases) {
		// Go up of the goal node.
		getNode(sf::Vector2u(p_Node->getGraphArrayPosition().x, p_Node->getGraphArrayPosition().y - numberOfDownBases)).setNodeState(NodeState::NOTHING);
		p_Node = &getNode(sf::Vector2u(p_Node->getGraphArrayPosition().x, p_Node->getGraphArrayPosition().y + numberOfUpBases));
	}
	else if (numberOfDownBases <= numberOfLeftBases && numberOfDownBases <= numberOfRightBases && numberOfDownBases <= numberOfUpBases && maxNumberOFNextBases >= numberOfDownBases) {
		// Go down of the goal node.
		getNode(sf::Vector2u(p_Node->getGraphArrayPosition().x, p_Node->getGraphArrayPosition().y - numberOfDownBases)).setNodeState(NodeState::NOTHING);
		p_Node = &getNode(sf::Vector2u(p_Node->getGraphArrayPosition().x, p_Node->getGraphArrayPosition().y - numberOfDownBases));
	}
	else {
		// Default!
		getNode(sf::Vector2u(20, 15)).setNodeState(NodeState::NOTHING);
		p_Node = &getNode(sf::Vector2u(20, 15));
	}

	return p_Node;
}

void Graph::setBaseNodes(const sf::Vector2u &p_BasePosition, int p_NeighbourSearchDistance) {
	bool alreadyHasBaseNode(false);
	for (auto &i : m_BasesFound) {
		if (i.x == p_BasePosition.x && i.y == p_BasePosition.y) {
			alreadyHasBaseNode = true;
			return;
		}
	}

	if (!alreadyHasBaseNode) {
		// Use the base position to set the nodes around it as "base" nodes, so the algorithm will not try to pass over them.
		int searchDistance = 20;	// Pixels.
		if (p_NeighbourSearchDistance < 20)
			searchDistance = p_NeighbourSearchDistance;

		for (int xPosition = p_BasePosition.x - p_NeighbourSearchDistance; xPosition < p_BasePosition.x + (p_NeighbourSearchDistance + p_NeighbourSearchDistance); xPosition += searchDistance) {
			for (int yPosition = p_BasePosition.y - p_NeighbourSearchDistance; yPosition < p_BasePosition.y + (p_NeighbourSearchDistance + p_NeighbourSearchDistance); yPosition += searchDistance) {
				if ((xPosition < 800  && xPosition > 0) && (yPosition < 600 && yPosition > 0))
					getPixelNode(sf::Vector2u(xPosition, yPosition)).setNodeState(NodeState::BASE);
			}
		}
		// Make sure we add the base, so we can always account for it. (Go by it.)
		m_BasesFound.push_back(p_BasePosition);
	}
}

void Graph::clearNodes() {
	// Go through every node and set their state to nothing, unless a base or wall is within the node.
	for (auto &i : m_Nodes) {
		for (auto &j : i) {
			if (!(j->getNodeState() == NodeState::BASE || j->getNodeState() == NodeState::WALL))
				j->setNodeState(NodeState::NOTHING);
			j->setParent(nullptr);
		}
	}
}

Node &Graph::getPixelNode(const sf::Vector2u &p_NodePixelPosition) const {
	// Give it a pixel position, it'll check every node's area, looking for that pixel within it (within the node's width/height).
	for (auto &i : m_Nodes) {
		for (auto &j : i) {
			if ((j->getPixelPosition().x - (j->getSize().x / 2) <= p_NodePixelPosition.x && j->getPixelPosition().x + (j->getSize().x / 2) >= p_NodePixelPosition.x)
				&& (j->getPixelPosition().y - (j->getSize().y / 2) <= p_NodePixelPosition.y && j->getPixelPosition().y + (j->getSize().y / 2) >= p_NodePixelPosition.y)) {
				return *j;
			}
		}
	}
}

Node &Graph::getNode(sf::Vector2u &p_NodeGraphPosition) {
	// Make sure a valid position has been selected.
	if (p_NodeGraphPosition.x < 0) 
		p_NodeGraphPosition.x = 0;
	else if (p_NodeGraphPosition.x >= s_m_ColumnSize) 
		p_NodeGraphPosition.x = s_m_ColumnSize - 1;
	if (p_NodeGraphPosition.y < 0) 
		p_NodeGraphPosition.y = 0;
	else if (p_NodeGraphPosition.y >= s_m_RowSize) 
		p_NodeGraphPosition.y = s_m_RowSize - 1;

	// Return the node.
	return *m_Nodes[p_NodeGraphPosition.y][p_NodeGraphPosition.x];
}

Node &Graph::getNode(int p_ID) const {
	// Get a node, using its associated ID.
	for (auto & i : m_Nodes) {
		for (auto & j : i) {
			if (j->getID() == p_ID)
				return *j;
		}
	}
}

NodeState Graph::getNodeState(sf::Vector2u p_NodeGraphPosition) {
	// Make sure a valid position has been selected.
	if (p_NodeGraphPosition.x < 0)
		p_NodeGraphPosition.x = 0;
	else if (p_NodeGraphPosition.x >= s_m_ColumnSize)
		p_NodeGraphPosition.x = s_m_ColumnSize - 1;
	if (p_NodeGraphPosition.y < 0)
		p_NodeGraphPosition.y = 0;
	else if (p_NodeGraphPosition.y >= s_m_RowSize)
		p_NodeGraphPosition.y = s_m_RowSize - 1;

	return m_Nodes[p_NodeGraphPosition.y][p_NodeGraphPosition.x]->getNodeState();
}

void Graph::setNodeState(const sf::Vector2u &p_NodeGraphPosition, const NodeState &p_NodeState) {
	m_Nodes[p_NodeGraphPosition.y][p_NodeGraphPosition.x]->setNodeState(p_NodeState);
}

std::vector<sf::Vector2u> &Graph::getBasesFound() {
	return m_BasesFound;
}

bool Graph::accountedForBase(const sf::Vector2u &p_Position) {
	// Searches through the basses we have stored, checking whether we have seen it before, and stored its position.
	// As if we've already seen it, then the nodes around it should be set to a BASE state.
	// So we've already accounted for the base, which means we won't have to recalculate the path, to the goal.
	for (auto &i : m_BasesFound) {
		if (i.x == p_Position.x && i.y == p_Position.y) {
			return true;
		}
	}

	return false;
}