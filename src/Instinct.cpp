#include "Instinct.h"
#include <iostream>

Instinct::Instinct() {
	clearMovement(); //Clear some weird movement bug
	//Debugging Enums
	m_eMainState = MainStates::Attacking;
	m_eAttackingState = AttackingStates::Attacking;
	

	//Node *tempNode = &m_Graph->getNode(sf::Vector2u(1, 20));
	//tempNode->setNodeState(NodeState::GOAL);
	//m_Path.push_back(tempNode);
	//m_eMainState = MainStates::Attacking;
	//m_eAttackingState = AttackingStates::Locating;
	//m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(getX(), getY())), m_Graph->getPixelNode(sf::Vector2u(300, 400)), m_Path);
}

Instinct::~Instinct() {
	m_Graph->~Graph();
}

void Instinct::reset() {
	//m_Path.clear();	// Clear the old path, ready for a new one.
}

void Instinct::move() { //called every frame
	//Debugging
	//takeAim();
	//drive();
	//QuadSearch();
}

void Instinct::collided() {

}

void Instinct::markTarget(Position p_Position) 
{
	Memorise(p_Position, false);
	m_Graph->setBaseNodes(sf::Vector2u(p_Position.getX(), p_Position.getY()));
	takeAim();
	
}

void Instinct::markEnemy(Position p_Position) {
	m_EnemyLastPosition = p_Position;
	takeAim();
}

void Instinct::markBase(Position p_Position) {
	Memorise(p_Position, true);
	takeAim();
	m_Graph->setBaseNodes(sf::Vector2u(p_Position.getX(), p_Position.getY()));
	m_bFriendlySeen = true;
	//if (!m_Graph->accountedForBase(sf::Vector2u(p_Position.getX(), p_Position.getY()))) {
	//
	//}
}

void Instinct::markShell(Position p_Position) {

}

bool Instinct::isFiring() {
	return m_bFiring;
}

void Instinct::score(int p_ThisScore, int p_EnemyScore) {

}

bool Instinct::drive()
{
	/*if (m_Path.size() == 0)
	{
		m_Graph->clearNodes();
		m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(getX(), getY())), m_Graph->getNode(sf::Vector2u(5, 15)), m_Path);
	}*/
	if (m_Path.size() != 0)
	{
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
			else
			{
				stop();
				return true;
			}
		}
	}
	return false;
}

bool Instinct::Scan(bool canScan)
{
	if (!canScan)
	{

		static bool scanStarted(false);
		static float startTurretAngle(turretTh);
		if (scanStarted == false)
		{
			scanStarted = true;
			startTurretAngle = turretTh;
			turretGoRight();
		}

		if (turretTh >= startTurretAngle - 5 && turretTh <= startTurretAngle - 1)
		{
			scanStarted = false;
			return true;
		}
		return false;
	}
	return true;
}

void Instinct::Memorise(Position p_BasePos, bool p_IsAlly)
{
	bool alreadyMemorised = false;
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

bool Instinct::QuadSearch() //TODO: nearest quad check has no way of resetig currently
{
	static bool nearestQuadCheck = false;
	static const Position quadrants[4] = {
		Position(195, 142), //upper left
		Position(585, 142), //upper right
		Position(585, 427), //lower right
		Position(195, 427)}; //lower left
	static int nearestQuad = 0;

	if (nearestQuadCheck == false)
	{
		nearestQuadCheck = true;
		float PrevDist = getDistance(quadrants[0]);
		for (int i = 0; i < sizeof(quadrants) / sizeof(*quadrants); i++) {
			float NextDist = getDistance(quadrants[i]);
			if (NextDist < PrevDist)
			{
				PrevDist = NextDist;
				nearestQuad = i;
			}
		}
	}

	static int quadCounter = 0;
	static int currentQuad = nearestQuad;
	static bool isTravelling = false;

	if (isTravelling == false)
	{
		std::cout << "!" << std::endl;
		m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())), m_Graph->getPixelNode(sf::Vector2u(quadrants[currentQuad].getX(), quadrants[currentQuad].getY())), m_Path);
		if (currentQuad == sizeof(quadrants) / sizeof(*quadrants) - 1)
		{
			currentQuad = 0;
		}
		else
		{
			currentQuad++;
		}
		isTravelling = true;
	}
	if (drive()) {
		quadCounter++;
		isTravelling = false;
	}
	if (quadCounter >= 3) // after reaching the start quad and anything afterwards you have finished
	{
		return true;
	}
	return false;
}

void Instinct::takeAim()
{
	/*float temp_enemy = getDistance(m_EnemyLastPosition);
	float temp_enemyBase = getDistance(m_EnemyBasePosition);*/

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

}

bool Instinct::takeAim(Position p_Position)
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
	}
	else if (deltaR < -1 && deltaR > -180 || deltaR > 180)
	{
		turretGoRight();
	}
	else
	{
		stopTurret();
		m_bTurretAligned = true;
	}
	return false;
}

void Instinct::fireEnemy(Position p_Position, bool isEnemy)
{

}

void Instinct::fireBases()
{
	//TODO - Pass bases into getDistance and sort based off that distance
	/*for (int i = 0; i < m_EnemyBases.size(); i++)
	{
		
	}*/

	takeAim(m_EnemyBases[0]);

	if (m_bTurretAligned)
	{
		m_bFiring = true;
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