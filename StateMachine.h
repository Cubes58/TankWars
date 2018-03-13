#pragma once

enum MainStates
{
	Defending,
	Attacking
};
enum DefendingStates
{
	Locating,
	Patrolling,
	Engaging,
	Neutral
};
enum AttackingStates
{
	Locating,
	Attacking,
	Neutral
};

class StateMachine
{
private:
	MainStates m_eMainState;
	DefendingStates m_eDefendingState;
	AttackingStates m_eAttackingState;



	bool m_bSeenBase = false;
	bool m_bEnemySeen = false;
	bool m_bBulletSeen = false;

	int m_iAmmoCount = 12;
	int m_iOurScore = 0;
	int m_iEnemyScore = 0;
public:
	StateMachine();
	~StateMachine();
};

