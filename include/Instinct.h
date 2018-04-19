#pragma once

#include "aitank.h"
#include "Graph.h"

enum class MainStates : unsigned int {
	Attacking,
	Engaging,
	Searching,
	Died
};

enum class SubStates : unsigned int {
	Driving,
	Scanning,
	PathFindNextNodes,
	PathFindNearNode,
	PathFindToNearBase,
	PathFindToLastEnemyPos,
	Aiming,
	Fire,
	ConfirmingBaseKill,
	Tracking,
	Neutral
};


class Instinct : public AITank {
private:
	Graph *m_Graph = new Graph;
	std::list<Node*> m_Path;
	Node* m_TargetNode;
	Node* m_CurrentNode;
	
	MainStates m_eMainState;
	MainStates m_ePreMainState;
	SubStates m_eSubState;

	sf::Vector2u m_AimingAt;

	bool m_bSeenBase = false;
	bool m_bCanScan = false;
	bool m_bEnemySeen = false;
	bool m_bFriendlySeen = false;
	bool m_bBulletSeen = false;
	bool m_bTurretAligned = false;
	bool m_bFiring = false;
	bool gotoCenter = false;
	bool m_enemyDied = false;
	bool m_enemyBaseDied = false;

	int m_iAmmoCountTotal = 15;
	int m_iOurScore = 0;
	int m_iEnemyScore = 0;
	int m_iEnemyBasesTotal = 10;
	int m_ixPos = 0;
	int m_iangleInDegrees = 0;
	int m_iturretAngle = 0;
	bool m_scanStarted = false;


	float rotPrecision = 5.0f;
	float m_fgetEnemyDistance = 0.0f;

	int basePointValue = 25;
	int tankPointValue = 10;
	int scoreThreshold = 30;

	std::vector<Position> m_AllyBases;
	std::vector<Position> m_EnemyBases;
	std::vector<Position> m_CanSee; 
	std::vector<Position> m_PreCanSee;

	bool m_bUncertain = true;
	bool m_EnemyGone = false;
	bool m_Fired = false;
	bool m_died = false;
	int m_iBaseCheckDiameter = 5;
	int m_targetQuad = 0;
	int m_QuadProgress = 0;


	std::list<Position> m_EnemyPos;
	//Position m_EnemyPrevPos;
	//std::list<Position> m_Enemy2FramePos;
	//std::vector<Position> m_CanSeeEnemy;

	const Position quadrants[4] = {
		Position(195, 142), //upper left
		Position(585, 142), //upper right
		Position(585, 427), //lower right
		Position(195, 427) }; //lower left

	const Position Centre = Position(400, 300) ;
public:
	Instinct();
	~Instinct();

	void reset();
	void move();
	void update();
	void markTarget(Position p_Position);
	void markEnemy(Position p_Position);
	void markBase(Position p_Position);
	void markShell(Position p_Position);
	void collided();
	bool isFiring();
	void score(int p_ThisScore, int p_EnemyScore);

	bool Drive();
	bool Scan(bool p_Clockwise);
	void Memorise(Position p_BasePos, bool p_IsAlly);
	//bool QuadSearch();
	void PathToNearNode();
	void PathToNextNode();
	void PathToNearBase();
	//void ReOrderBases();
	bool isBaseDestroyed();
	bool isTracked();

	//void takeAim();
	bool takeAim(float x, float y);
	bool fire();
	float getDistance(Position p_Position);

	Graph *getGraph() const;
};