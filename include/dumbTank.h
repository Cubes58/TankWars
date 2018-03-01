#ifndef DUMB_TANK_H
#define DUMB_TANK_H

#include "aitank.h"


class DumbTank : public AITank
{
private:
	bool forwards;
public:
    DumbTank();
    ~DumbTank();

	void move();
	void reset();
	void collided();
	void markTarget(Position p);
	void markEnemy(Position p);
	void markBase(Position p);
	void markShell(Position p);
	bool isFiring(); 
	void score(int thisScore,int enemyScore);
};
#endif