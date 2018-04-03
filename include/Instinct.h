#pragma once

#include "aitank.h"
#include "Graph.h"

enum class MainStates : unsigned int {
	Defending,
	Attacking
};
enum class DefendingStates : unsigned int {
	Locating,
	Patrolling,
	Engaging,
	Neutral
};
enum class AttackingStates : unsigned int {
	Locating,
	Attacking,
	Neutral
};

class Instinct : public AITank {
private:
	Graph *m_Graph = new Graph;
	std::list<Node*> m_Path;
	Node m_TargetNode;
	Node m_CurrentNode;
	
	MainStates m_eMainState;
	DefendingStates m_eDefendingState;
	AttackingStates m_eAttackingState;

	bool m_bSeenBase = false;
	bool m_bEnemySeen = false;
	bool m_bBulletSeen = false;

	int m_iAmmoCount = 12;
	int m_iOurScore = 0;
	int m_iEnemyScore = 0;

	bool m_bFiring = false;

	Position m_EnemyLastPosition;
public:
	Instinct();
	~Instinct();

	void reset();
	void move();
	void collided();
	void markTarget(Position p_Position);
	void markEnemy(Position p_Position);
	void markBase(Position p_Position);
	void markShell(Position p_Position);
	bool isFiring();
	void score(int p_ThisScore, int p_EnemyScore);

	void drive();

	Graph *getGraph() const;
};