#include "Instinct.h"
#include <iostream>
#include <functional>
#include <algorithm>

Instinct::Instinct() {
	clearMovement(); //Clear some weird movement bug
	//Initial states
	m_eMainState = MainStates::Searching;
	m_eSubState = SubStates::PathFindNearNode;
}

Instinct::~Instinct() {
	m_Graph->~Graph();
}

void Instinct::reset() {
	
}

void Instinct::move() { //called every frame
	update();
}

void Instinct::update()
{
	m_bFiring = false;
	if ((m_eMainState == MainStates::Searching && m_eSubState == SubStates::Neutral) || m_eMainState != MainStates::Searching)
	{
		if (m_iOurScore < m_iEnemyScore)
		{
			m_eMainState = MainStates::Defending;
		}
		else if (m_eMainState == MainStates::Defending && m_iOurScore >= m_iEnemyScore + scoreThreshold)
		{
			m_eMainState = MainStates::Attacking;
			m_eSubState = SubStates::PathFindToNearBase;
		}
		else if (m_eMainState == MainStates::Searching && m_iOurScore >= m_iEnemyScore) {
			m_eMainState = MainStates::Attacking;
			m_eSubState = SubStates::PathFindToNearBase;
		}
	}

	m_ePreMainState = m_eMainState;
	switch (m_eMainState) {
	case MainStates::Searching:
		std::cout << "Searching" << std::endl;
		switch (m_eSubState) {
		case SubStates::PathFindNearNode:		//find path to nearest quad node
			std::cout << "finding path to nearest quad" << std::endl;
			PathToNearNode();						//pathfind
			m_eSubState = SubStates::Driving;	//set driving to do after pathing
			break;
		case SubStates::PathFindNextNodes:	//find path to next quad node
			std::cout << "finding path to next quad" << std::endl;
			PathToNextNode();	//pathfind
			m_eSubState = SubStates::Driving;	//set driving to do after pathing
			break;
		case SubStates::Scanning:	//scan area
			std::cout << "scanning quad" << std::endl;
			if (Scan(false)) {	//if scan complete
				m_QuadProgress++;
				m_eSubState = SubStates::PathFindNextNodes;	//set pathing to do after scan
			} 
			break;
		case SubStates::Driving:	//drive to next quad node
			std::cout << "driving" << std::endl;
			if (Drive()) {	//if drive complete
				if (m_QuadProgress >= sizeof(quadrants) / sizeof(*quadrants)) {
					m_eSubState = SubStates::Neutral;
				}
				else {
					m_eSubState = SubStates::Scanning;	//set scan to do after drive
				}
			}
			break;
		default:
			break;
		}
		break;
	case MainStates::Attacking:
		std::cout << "Attacking" << std::endl;
		switch (m_eSubState) {
		case SubStates::PathFindToNearBase:
			std::cout << "finding path near to base" << std::endl;
			m_Graph->aStarSearchAlgorithm(&m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())), &m_Graph->getPixelNode(sf::Vector2u(m_EnemyBases.back().getX(), m_EnemyBases.back().getY())), m_Path);
			m_eSubState = SubStates::Driving;	//set driving to do after pathing
			break;
		case SubStates::Driving:
			std::cout << "driving" << std::endl;
			if (Drive()) {
				m_eSubState = SubStates::Aiming;
			}
			break;
		case SubStates::Aiming:
			std::cout << "aiming" << std::endl;
			if (takeAim()) {
				m_eSubState = SubStates::Fire;
			}
			break;
		case SubStates::Fire:
			std::cout << "firing" << std::endl;
			if (fire()) {
				m_eSubState = SubStates::ConfirmingBaseKill;
			}
			break;
		case SubStates::ConfirmingBaseKill:
			std::cout << "confirming all bases in cluster are destroyed" << std::endl;
			if (isBaseDestroyed()) {
				if (m_EnemyBases.size() != 0) {
					if (m_CanSee.size() == 0) {
						m_eSubState = SubStates::PathFindToNearBase;
					}
					else {
						m_eSubState = SubStates::Aiming;
					}
				}
				else {
					m_eSubState = SubStates::Neutral;	//game is over?
				}
			}
		default:
			break;
		}
		break;
	case MainStates::Defending:
		switch (m_eSubState) {
		//////////////
		default:
			break;
		}
		break;
	case MainStates::Died:
		switch (m_ePreMainState) {
		case MainStates::Searching:
			m_QuadProgress = 0;
			m_eSubState = SubStates::PathFindNearNode;
			break;
		case MainStates::Attacking:

			break;
		case MainStates::Defending:

			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	m_PreCanSee.clear();
	m_PreCanSee = m_CanSee;
	m_CanSee.clear();
}

void Instinct::markTarget(Position p_Position) 
{
	Memorise(p_Position, false);
	m_CanSee.push_back(p_Position);
}

void Instinct::markEnemy(Position p_Position) {
	m_EnemyLastPosition = p_Position;
}

void Instinct::markBase(Position p_Position) {
	Memorise(p_Position, true);
}

void Instinct::markShell(Position p_Position) {

}

void Instinct::collided()
{
}

bool Instinct::isFiring() {
	return m_bFiring;
}

void Instinct::score(int p_ThisScore, int p_EnemyScore) 
{
	int deltaOurScore = p_ThisScore - m_iOurScore;
	int deltaEnemyScore = p_EnemyScore - m_iEnemyScore;
	//check changes in the enemies score
	if (deltaEnemyScore == 25) {
		m_eMainState = MainStates::Died;
	}
	//check changes in our score
	if (deltaOurScore == 25) {
		m_enemyDied = true;	//they died
	}
	else if (deltaOurScore == 10) {
		//m_EnemyBases.pop_back();
	}
	//depending on the current score change our main state

	m_iOurScore = p_ThisScore;
	m_iEnemyScore = p_EnemyScore;
}

bool Instinct::Drive()
{
	if (m_Path.size() != 0)
	{
		if (m_bUncertain) {
			m_Graph->aStarSearchAlgorithm(&m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())), m_Path.front(), m_Path);
			m_bUncertain = false;
		}

		//currentNode = get node from graph using pixel pos
		m_CurrentNode = &m_Graph->getPixelNode(sf::Vector2u(getX(), getY()));
		m_TargetNode = m_Path.back();

		//move here
		int deltaY = getY() - m_TargetNode->getPixelPosition().y;
		int deltaX = getX() - m_TargetNode->getPixelPosition().x;
		float dTargetAngle = (atan2(deltaY, deltaX) * 180 / PI) + 180;
		turretTh;
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
		}
	}
	else
	{
		stop();
		return true;
	}
	return false;
}

bool Instinct::Scan(bool p_Clockwise)
{
	static bool scanStarted = false;
	static float startTurretAngle = turretTh;
	static float deltaAngle = 0.0f;

	if (!scanStarted) {
		scanStarted = true;
		startTurretAngle = turretTh;
		if (p_Clockwise) {
			turretGoRight();
			goRight();
		}
		else {
			turretGoLeft();
			goLeft();
		}
	}

	if (p_Clockwise) {
		if (turretTh >= startTurretAngle) {
			deltaAngle = turretTh - startTurretAngle;
		}
		else {
			deltaAngle = 360 - abs(turretTh - startTurretAngle);
		}
	}
	else {
		if (turretTh <= startTurretAngle) {
			deltaAngle = abs(turretTh - startTurretAngle);
		}
		else {
			deltaAngle = 360 - (turretTh - startTurretAngle);
		}
	}

	std::cout << deltaAngle << std::endl;

	if (deltaAngle < 360 && deltaAngle > 355) {
		scanStarted = false;
		stop();
		stopTurret();
		std::cout << "scan finished" << std::endl;
		return true;
	}
	return false;
}

void Instinct::Memorise(Position p_BasePos, bool p_IsAlly)
{
	bool alreadyMemorised = false;
	if (!m_Graph->accountedForBase(sf::Vector2u(p_BasePos.getX(), p_BasePos.getY()))) {
		m_bUncertain = true;
		m_Graph->setBaseNodes(sf::Vector2u(p_BasePos.getX(), p_BasePos.getY()));
	}
	if (p_IsAlly)
	{
		if (m_AllyBases.size() != 0)
		{
			for (int i = 0; i < m_AllyBases.size(); i++)
			{
				if (p_BasePos.getX() == m_AllyBases[i].getX() && p_BasePos.getY() == m_AllyBases[i].getY()) {
					alreadyMemorised = true;
				}
			}
			if (!alreadyMemorised) {
				m_AllyBases.push_back(p_BasePos);
			}
		}
		else
		{
			m_AllyBases.push_back(p_BasePos);
		}
	}
	else
	{
		if (m_EnemyBases.size() != 0)
		{
			for (int i = 0; i < m_EnemyBases.size(); i++)
			{
				if (p_BasePos.getX() == m_EnemyBases[i].getX() && p_BasePos.getY() == m_EnemyBases[i].getY()) {
					alreadyMemorised = true;
				}
			}
			if (!alreadyMemorised) {
				m_EnemyBases.push_back(p_BasePos);
			}
		}
		else
		{
			m_EnemyBases.push_back(p_BasePos);
		}
	}
}

void Instinct::PathToNearNode()
{
	
	float PrevDist = getDistance(quadrants[0]);
	for (int i = 0; i < sizeof(quadrants) / sizeof(*quadrants); i++) {
		float NextDist = getDistance(quadrants[i]);
		if (NextDist < PrevDist)
		{
			PrevDist = NextDist;
			m_targetQuad = i;
		}
	}
	m_Graph->aStarSearchAlgorithm(&m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())), &m_Graph->getPixelNode(sf::Vector2u(quadrants[m_targetQuad].getX(), quadrants[m_targetQuad].getY())), m_Path);
}

void Instinct::PathToNextNode()
{
	if (m_targetQuad == sizeof(quadrants) / sizeof(*quadrants) - 1) {
		m_targetQuad = 0;
	}
	else
	{
		m_targetQuad++;
	}
	m_Graph->aStarSearchAlgorithm(&m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())), &m_Graph->getPixelNode(sf::Vector2u(quadrants[m_targetQuad].getX(), quadrants[m_targetQuad].getY())), m_Path);
}

void Instinct::PathToNearBase()
{
	m_Graph->aStarSearchAlgorithm(&m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())), &m_Graph->getPixelNode(sf::Vector2u(m_EnemyBases.back().getX(), m_EnemyBases.back().getY())), m_Path);
}
/*
void Instinct::ReOrderBases()
{
	float PrevDist = getDistance(m_EnemyBases[0]);
	std::list<Position> tempPositions;
	for (int i = 0; i < m_EnemyBases.size(); i++) {
		float NextDist = getDistance(m_EnemyBases[i]);
		if (NextDist < PrevDist)
		{
			tempPositions.push_back(m_EnemyBases[i]);
			m_EnemyBases.erase(m_EnemyBases.begin() + i);
		}
		else {
			tempPositions.push_front(m_EnemyBases[i]);
			m_EnemyBases.erase(m_EnemyBases.begin() + i);
		}
		PrevDist = NextDist;
	}
	for (int i = 0; i < tempPositions.size(); i++) {
		m_EnemyBases.push_back(tempPositions.front());
		tempPositions.pop_back();
	}
}*/

bool Instinct::isBaseDestroyed()
{
	static bool seen = false;
	for (int i = 0; i < m_PreCanSee.size(); i++) {
		seen = false;
		for (int j = 0; j < m_CanSee.size(); j++) {
			if (m_PreCanSee[i].getX() == m_CanSee[j].getX() && m_CanSee[j].getY() == m_PreCanSee[i].getY()) {
				seen = true;
			}
		}
		if (!seen) {
			for (int k = 0; k < m_EnemyBases.size(); k++) {
				if (m_PreCanSee[i].getX() == m_EnemyBases[k].getX() && m_EnemyBases[k].getY() == m_PreCanSee[i].getY()) {
					m_EnemyBases.erase(m_EnemyBases.begin() + k);
				}
			}
			return true;
		}
	}
	return false;
}

bool Instinct::takeAim()
{
	static bool aimStarted = false;
	static float deltaX = 0.0f;
	static float deltaY = 0.0f;
	static float theta = 0.0f;
	static float deltaAngle = 0.0f;
	if (!aimStarted) {
		deltaX = getX() - m_CanSee.back().getX();
		deltaY = getY() - m_CanSee.back().getY();
		theta = (atan2(deltaY, deltaX) * 180 / PI) + 180;
	}
	deltaAngle = turretTh - theta;
	if (deltaAngle > 1 && deltaAngle < 180 || deltaAngle < -180)
	{
		turretGoLeft();
	}
	else if (deltaAngle < -1 && deltaAngle > -180 || deltaAngle > 180)
	{
		turretGoRight();
	}
	else
	{
		stopTurret();
		return true;
	}
	return false;
}

bool Instinct::fire()
{
	if (canFire()) {
		m_bFiring = true;
		return true;
	}
	return false;
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