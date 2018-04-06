#include "Instinct.h"
#include <iostream>

Instinct::Instinct() {
	clearMovement(); //Clear some weird movement bug

	//Node *tempNode = &m_Graph->getNode(sf::Vector2u(1, 20));
	//tempNode->setNodeState(NodeState::GOAL);
	//m_Path.push_back(tempNode);
	//m_eMainState = MainStates::Attacking;
	//m_eAttackingState = AttackingStates::Locating;
	//m_Graph->aStarSearchAlgorithm(*m_Graph->getPixelNode(sf::Vector2u(getX(), getY())), m_Graph->getNode(sf::Vector2u(5, 15)), m_Path);
}

Instinct::~Instinct() {
	m_Graph->~Graph();
}

void Instinct::reset() {
	//m_Path.clear();	// Clear the old path, ready for a new one.
}

void Instinct::move() { //called every frame
	//Scan();
	drive();
}

void Instinct::collided() {

}

void Instinct::markTarget(Position p_Position) {

	m_EnemyBasePosition = p_Position;

	m_bEnemySeen = true;
	m_ixPos = p_Position.getX();

	float deltaX = getX() - p_Position.getX();
	float deltaY = getY() - p_Position.getY();

	m_iangleInDegrees = atan2(deltaY, deltaX) * 180 / PI;
	m_iturretAngle = m_iangleInDegrees + 180;

	takeAim();
	
}

void Instinct::markEnemy(Position p_Position) {
	m_EnemyLastPosition = p_Position;
	takeAim();
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
	if (m_Path.size() == 0)
	{
		m_Graph->aStarSearchAlgorithm(*m_Graph->getPixelNode(sf::Vector2u(getX(), getY())), m_Graph->getNode(sf::Vector2u(5, 15)), m_Path);
	}
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
		bool finishedDriving = m_Path.size() == 0;

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

void Instinct::Scan()
{
	static bool scanStarted(false);
	static float startTurretAngle(turretTh);
	if (scanStarted == false)
	{
		scanStarted = true;
		startTurretAngle = turretTh;
		if (debugMode)
		{
			std::cout << startTurretAngle << std::endl;
		}
		turretGoRight();
	}
	if (turretTh >= startTurretAngle - 5 && turretTh <= startTurretAngle)
	{
		if (debugMode)
		{
			std::cout << "scan finished" << std::endl;
		}
		scanStarted = false;
	}
	if (scanStarted && debugMode)
	{
		std::cout << turretTh << std::endl; //0 is to the east
	}
}

void Instinct::takeAim()
{
	float temp_enemy = getDistance(m_EnemyLastPosition);
	float temp_enemyBase = getDistance(m_EnemyBasePosition);

	if (temp_enemy < temp_enemyBase)
	{

		m_bEnemySeen = true;
		m_ixPos = m_EnemyLastPosition.getX();

		float deltaX = getX() - m_EnemyLastPosition.getX();
		float deltaY = getY() - m_EnemyLastPosition.getY();

		m_iangleInDegrees = atan2(deltaY, deltaX) * 180 / PI;
		m_iturretAngle = m_iangleInDegrees + 180;

		float deltaR = turretTh - m_iturretAngle;

		if (deltaR > 1 && deltaR < 180 || deltaR < -180)
		{
			turretGoLeft();
		}
		else if (deltaR < -1 && deltaR > -180 || deltaR > 180)
		{
			turretGoRight();
		}
		else
		{
			m_bTurretAligned = true;
			stopTurret();
		}
	}
	else
	{
		m_bEnemySeen = true;
		m_ixPos = m_EnemyBasePosition.getX();

		float deltaX = getX() - m_EnemyBasePosition.getX();
		float deltaY = getY() - m_EnemyBasePosition.getY();

		m_iangleInDegrees = atan2(deltaY, deltaX) * 180 / PI;
		m_iturretAngle = m_iangleInDegrees + 180;

		float deltaR = turretTh - m_iturretAngle;

		if (deltaR > 1 && deltaR < 180 || deltaR < -180)
		{
			turretGoLeft();
		}
		else if (deltaR < -1 && deltaR > -180 || deltaR > 180)
		{
			turretGoRight();
		}
		else
		{
			m_bTurretAligned = true;
			stopTurret();
		}
	}

}

float Instinct::getDistance(Position p_Position)
{
	float dx = (float)(p_Position.getX() - getX());
	float dy = (float)(p_Position.getY() - getY());

	float temp_Distance = sqrt(dx * dx + dy * dy);

	return temp_Distance;
}



Graph *Instinct::getGraph() const {
	return m_Graph;
}