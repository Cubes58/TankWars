#include "Instinct.h"

Instinct::Instinct() {
	clearMovement(); //Clear some weird movement bug
	//Node *tempNode = &m_Graph->getNode(sf::Vector2u(25, 20));
	Node *tempNode1 = &m_Graph->getNode(sf::Vector2u(5, 15));
	//Node *tempNode2 = &m_Graph->getNode(sf::Vector2u(10, 20));
	//m_Path.push_back(tempNode);
	m_Path.push_back(tempNode1);
	//m_Path.push_back(tempNode2);
	m_eMainState = MainStates::Attacking;
	m_eAttackingState = AttackingStates::Locating;
}

Instinct::~Instinct() {
	m_Graph->~Graph();
}

void Instinct::reset() {
	//m_Path.clear();	// Clear the old path, ready for a new one.
}

void Instinct::move() { //called every frame
	if (m_eMainState == MainStates::Attacking && m_eAttackingState == AttackingStates::Locating)
	{
		drive();
	}
}

void Instinct::collided() {

}

void Instinct::markTarget(Position p_Position) {
	
}

void Instinct::markEnemy(Position p_Position) {
	m_EnemyLastPosition = p_Position;
/*
	m_bEnemySeen = true;
	m_ixPos = p_Position.getX();

	float deltaX = getX() - p_Position.getX();
	float deltaY = getY() - p_Position.getY();

	m_iangleInDegrees = atan2(deltaY, deltaX) * 180 / PI;
	m_iturretAngle = m_iangleInDegrees + 180;

	float dx = (float)(p_Position.getX() - getX());
	float dy = (float)(p_Position.getY() - getY());

	m_fgetEnemyDistance = sqrt(dx * dx + dy * dy);
	*/
}

void Instinct::markBase(Position p_Position) {

}

void Instinct::markShell(Position p_Position) {

}

bool Instinct::isFiring() {
	return m_bFiring;
}

void Instinct::score(int p_ThisScore, int p_EnemyScore) {

}

void Instinct::drive()
{
	if (m_Path.size() != 0)
	{
		//currentNode = getnode from graph using pixel pos
		m_CurrentNode = m_Graph->getPixelNode(sf::Vector2u(getX(), getY()));
		m_TargetNode = m_Path.back();

		//move here
		int deltaY = getY() - m_TargetNode->getPosition().y;
		int deltaX = getX() - m_TargetNode->getPosition().x;
		float dTargetAngle = (atan2(deltaY, deltaX) * 180 / PI) + 180;
		float dTankAngle = pos.getTh();
		float dDeltaAngle = dTankAngle - dTargetAngle;
		bool facingTarget = (dDeltaAngle < rotPrecision && dDeltaAngle > -rotPrecision);
		bool finishedDriving = (m_Path.size() == 0);

		if (facingTarget && !finishedDriving)
		{
			goForward();
		}
		else if (!facingTarget && !finishedDriving)
		{
			if (dDeltaAngle > 1 && dDeltaAngle < 180) {
				goLeft();
			}
			else if (dDeltaAngle < -1 && dDeltaAngle > -180) {
				goRight();
			}
			else if (dDeltaAngle > 180) {
				goRight();
			}
			else if (dDeltaAngle < -180) {
				goLeft();
			}
		}

		//check if we reach a path node
		if (m_CurrentNode->getGraphArrayPosition().x == m_TargetNode->getGraphArrayPosition().x && m_CurrentNode->getGraphArrayPosition().y == m_TargetNode->getGraphArrayPosition().y)
		{
			m_Path.pop_back();
			if (m_Path.size() != 0)
			{
				m_TargetNode = m_Path.back();
			}
			else
			{
				stop();
			}
		}
	}
}

Graph *Instinct::getGraph() const {
	return m_Graph;
}