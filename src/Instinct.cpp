#include "Instinct.h"
#include <iostream>
#include <functional>
#include <algorithm>

Instinct::Instinct() {
	clearMovement(); //Clear some weird movement bug
	//Debugging Enums
	m_eMainState = MainStates::Attacking;
	m_eAttackingState = AttackingStates::Locating;
	m_eDefendingState = DefendingStates::Neutral;
}

Instinct::~Instinct() {
	m_Graph->~Graph();
}

void Instinct::reset() {
	
}

void Instinct::move() { //called every frame
	//update();
	//drive();
	//QuadSearch();
	Scan();
}

void Instinct::update()
{
	if (m_eMainState == MainStates::Attacking)
	{
		if (QuadSearch() && m_eAttackingState == AttackingStates::Locating)
		{
			gotoCenter = false;
			m_eAttackingState = AttackingStates::Attacking;
		}
		if (m_eAttackingState == AttackingStates::Attacking)
		{
			if (!gotoCenter)
			{
				m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(getX(), getY())), m_Graph->getPixelNode(sf::Vector2u(300, 540)), m_Path);
				gotoCenter = true;
			}
			else
			{
				fireBases();
			}

		}
	}
	if (m_eMainState == MainStates::Defending)
	{
		gotoCenter = false;
		if (m_bEnemySeen)
		{
			m_eDefendingState = DefendingStates::Engaging;
		}
	}
}

void Instinct::markTarget(Position p_Position) 
{
	Memorise(p_Position, false);
	//if (!m_Graph->accountedForBase(sf::Vector2u(p_Position.getX(), p_Position.getY()))) {
	//	m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(getX(), getY())), m_Graph->getPixelNode(sf::Vector2u(300, 540)), m_Path);
	//}
	//m_Graph->setBaseNodes(sf::Vector2u(p_Position.getX(), p_Position.getY()));
}

void Instinct::markEnemy(Position p_Position) {
	m_EnemyLastPosition = p_Position;
}

void Instinct::markBase(Position p_Position) {
	Memorise(p_Position, true);
	//if (!m_Graph->accountedForBase(sf::Vector2u(p_Position.getX(), p_Position.getY()))) {
		//m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(getX(), getY())), m_Graph->getPixelNode(sf::Vector2u(300, 240)), m_Path);
	//}
	//m_Graph->setBaseNodes(sf::Vector2u(p_Position.getX(), p_Position.getY()));
	//m_bFriendlySeen = true;
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
	if (p_EnemyScore >= p_ThisScore + 30)
	{
		m_eMainState = MainStates::Defending;
	}
	else
	{
		m_eMainState = MainStates::Attacking;
	}
	if (p_ThisScore - m_iOurScore == 10)
	{
		m_baseHit = true;
	}
	if (p_ThisScore - m_iOurScore == 25)
	{
		m_enemyHit = true;
	}
	else
	{
		m_enemyHit = false;
		m_baseHit = false;
	}
	m_iOurScore = p_ThisScore;
	m_iEnemyScore = p_EnemyScore;
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
			//else
			//{
			//	stop();
			//	return true;
			//}
		}
	}
	else
	{
		stop();
		return true;
	}
	return false;
}

bool Instinct::Scan()
{
	static bool scanStarted(false);
	static float startTurretAngle(turretTh);
	if (!scanStarted)
	{
		scanStarted = true;
		startTurretAngle = turretTh;
		turretGoRight();
	} else if (turretTh >= startTurretAngle - 5 && turretTh <= startTurretAngle - 1) {
		scanStarted = false;
		std::cout << "scan finished" << std::endl;
		return true;
	}
	return true;
}

void Instinct::Memorise(Position p_BasePos, bool p_IsAlly)
{
	bool alreadyMemorised = false;
	if (!m_Graph->accountedForBase(sf::Vector2u(p_BasePos.getX(), p_BasePos.getY()))) {
		m_bUncertain = true;
	}
	m_Graph->setBaseNodes(sf::Vector2u(p_BasePos.getX(), p_BasePos.getY()));
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

bool Instinct::QuadSearch() //TODO: nearest quad check has no way of reseting currently
{
	static bool nearestQuadCheck = false;
	static const Position quadrants[4] = {
		Position(195, 142), //upper left
		Position(585, 142), //upper right
		Position(585, 427), //lower right
		Position(195, 427) }; //lower left
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
		Node goalNode = m_Graph->getPixelNode(sf::Vector2u(quadrants[currentQuad].getX(), quadrants[currentQuad].getY()));
		if (m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())),
			goalNode, m_Path)) {
			bool first = true;
			Node* node = nullptr;
			Node* bestNode = nullptr;
			for (int i = -m_iBaseCheckDiameter; i <= m_iBaseCheckDiameter; i++)
			{
				for (int j = -m_iBaseCheckDiameter; j <= m_iBaseCheckDiameter; j++)
				{
					node = &m_Graph->getNode(goalNode.getGraphArrayPosition() + sf::Vector2u(i, j));
					if (first) {
						bestNode = node;
						first = false;
					}
					else {
						if (getDistance(Position(node->getPixelPosition().x, node->getPixelPosition().y)) < getDistance(Position(bestNode->getPixelPosition().x, bestNode->getPixelPosition().y)) && node->getNodeState() != NodeState::BASE) {
							bestNode = node;
						}
					}
				}
			}
			m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())),
				*bestNode, m_Path);
		 }
		isTravelling = true;
	}

	if (m_bUncertain) {
		Node goalNode = m_Graph->getPixelNode(sf::Vector2u(quadrants[currentQuad].getX(), quadrants[currentQuad].getY()));
		if (goalNode.getNodeState() == NodeState::BASE)
		{
			bool first = true;
			Node* node = nullptr;
			Node* bestNode = nullptr;
			for (int i = -m_iBaseCheckDiameter; i <= m_iBaseCheckDiameter; i++)
			{
				for (int j = -m_iBaseCheckDiameter; j <= m_iBaseCheckDiameter; j++)
				{
					node = &m_Graph->getNode(goalNode.getGraphArrayPosition() + sf::Vector2u(i, j));
					if (first) {
						bestNode = node;
						first = false;
					} else {
						if (getDistance(Position(node->getPixelPosition().x, node->getPixelPosition().y)) < getDistance(Position(bestNode->getPixelPosition().x, bestNode->getPixelPosition().y)) && node->getNodeState() != NodeState::BASE) {
							bestNode = node;
						}
					}
				}
			}
			m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())),
				*bestNode, m_Path);
		}
		else
		{
			m_Graph->aStarSearchAlgorithm(m_Graph->getPixelNode(sf::Vector2u(this->getX(), this->getY())),
				m_Graph->getPixelNode(sf::Vector2u(quadrants[currentQuad].getX(), quadrants[currentQuad].getY())), m_Path);
		}
		m_bUncertain = false;
	}

	if (drive()) {
		quadCounter++;
		if (currentQuad == sizeof(quadrants) / sizeof(*quadrants) - 1)
		{
			currentQuad = 0;
		}
		else
		{
			currentQuad++;
		}
		isTravelling = false;
	}
	if (quadCounter >= 3) // after reaching the start quad and anything afterwards you have finished
	{
		return true;
	}
	return false;
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
			if (m_baseHit)
			{
				m_EnemyBases.erase(m_EnemyBases.begin());
			}
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