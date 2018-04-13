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
	if ((m_eMainState == MainStates::Searching && m_eSubState == SubStates::Neutral) || m_eMainState != MainStates::Searching)
	{
		if (m_iOurScore < m_iEnemyScore)
		{
			m_eMainState = MainStates::Defending;
		}
		else if (m_iOurScore > m_iEnemyScore + scoreThreshold)
		{
			m_eMainState = MainStates::Attacking;
		}
	}

	m_ePreMainState = m_eMainState;
	switch (m_eMainState) {
	case MainStates::Searching:
		switch (m_eSubState) {
		case SubStates::PathFindNearNode:		//find path to nearest quad node
			PathToNearNode();						//pathfind
			m_eSubState = SubStates::Driving;	//set driving to do after pathing
			break;
		case SubStates::PathFindNextNodes:	//find path to next quad node
			PathToNextNode();	//pathfind
			if (m_QuadProgress >= sizeof(quadrants) / sizeof(*quadrants)) {
				m_eSubState = SubStates::Neutral;
			}
			else
			{
				m_eSubState = SubStates::Driving;	//set driving to do after pathing
			}
			break;
		case SubStates::Scanning:	//scan area
			if (Scan(false)) {	//if scan complete
				m_QuadProgress++;
				m_eSubState = SubStates::PathFindNextNodes;	//set pathing to do after scan
			} 
			break;
		case SubStates::Driving:	//drive to next quad node
			if (Drive()) {	//if drive complete
				m_eSubState = SubStates::Scanning;	//set scan to do after drive
			}
			break;
		default:
			break;
		}
		break;
	case MainStates::Attacking:
		switch (m_eSubState) {
		/////////////
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
}

void Instinct::markTarget(Position p_Position) 
{
	Memorise(p_Position, false);
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
		m_enemyBaseDied = true;	//we destroyed a base
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
			m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())), *m_Path.front(), m_Path);
			m_bUncertain = false;
		}

		//currentNode = get node from graph using pixel pos
		m_CurrentNode = &m_Graph->getPixelNode(sf::Vector2u(getX(), getY()));
		m_TargetNode = m_Path.back();

		//move here
		int deltaY = getY() - m_TargetNode->getPixelPosition().y;
		int deltaX = getX() - m_TargetNode->getPixelPosition().x;
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
	m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())), m_Graph->getPixelNode(sf::Vector2u(quadrants[m_targetQuad].getX(), quadrants[m_targetQuad].getY())), m_Path);
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
	m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())), m_Graph->getPixelNode(sf::Vector2u(quadrants[m_targetQuad].getX(), quadrants[m_targetQuad].getY())), m_Path);
}

/*void Instinct::takeAim()
{
	float temp_enemy = getDistance(m_EnemyLastPosition);
	float temp_enemyBase = getDistance(m_EnemyBasePosition);

		takeAim(m_EnemyLastPosition);
		if (m_eMainState == MainStates::Defending && m_eDefendingState == DefendingStates::Engaging)
		{
			fireEnemy(m_EnemyLastPosition, m_bEnemySeen);
		}

		if (m_eMainState == MainStates::Attacking && m_eAttackingState == AttackingStates::Attacking)
		{
			//takeAim(m_EnemyBasePosition);
			fireBases();
		}

}*/

void Instinct::takeAim(Position p_Position)
{
	m_ixPos = p_Position.getX();

	float deltaX = getX() - p_Position.getX();
	float deltaY = getY() - p_Position.getY();

	m_iangleInDegrees = atan2(deltaY, deltaX) * 180 / PI;
	m_iturretAngle = m_iangleInDegrees + 180;

	float deltaR = turretTh - m_iturretAngle;

	if (deltaR > 1 && deltaR < 180 || deltaR < -180)
	{
		turretGoLeft();
		m_bTurretAligned = false;
	}
	else if (deltaR < -1 && deltaR > -180 || deltaR > 180)
	{
		turretGoRight();
		m_bTurretAligned = false;
	}
	else
	{
		stopTurret();
		m_bTurretAligned = true;
	}
}

void Instinct::fireEnemy(Position p_Position, bool isEnemy)
{

}

void Instinct::fireBases()
{
	//TODO - Pass bases into getDistance and sort based off that distance
	/*for (int i = 0; i < m_EnemyBases.size(); i++)
	{
		getDistance(m_EnemyBases[i]);
	}*/

	//std::stable_sort(m_EnemyBases.begin(), m_EnemyBases.end(), std::mem_fun_ref(&Instinct::getDistance));
	if (m_EnemyBases.size() != 0)
	{
		takeAim(m_EnemyBases[0]);

		if (m_bTurretAligned)
		{
			m_bFiring = true;
			//if (m_baseHit)
			//{
			//	m_EnemyBases.erase(m_EnemyBases.begin());
			//}
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