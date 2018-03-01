#include "DumbTank.h"

DumbTank::DumbTank() // Construtor
{
	forwards = true;
}

DumbTank::~DumbTank(){} // Destructor


void DumbTank::reset()
{
	forwards = true;
}

void DumbTank::move()
{
	if(forward)
	{
		goForward();
		turretGoRight();
	}
	else
	{
		goBackward();
		turretGoLeft();
	}
}

void DumbTank::collided()
{
	forwards = !forwards;
}

void DumbTank::markTarget(Position p)
{
	std::cout << "Target spotted at (" <<p.getX() << ", " << p.getY() << ")\n"; 
}

void DumbTank::markEnemy(Position p)
{
	std::cout << "Enemy spotted at (" <<p.getX() << ", " << p.getY() << ")\n"; 
}

void DumbTank::markBase(Position p)
{
	std::cout << "Base spotted at (" <<p.getX() << ", " << p.getY() << ")\n"; 
}

void DumbTank::markShell(Position p)
{
	std::cout << "Shell spotted at (" <<p.getX() << ", " << p.getY() << ")\n"; 
}

bool DumbTank::isFiring()
{
	return !forwards; // Fire when going backwards
} 

void DumbTank::score(int thisScore,int enemyScore)
{
	std::cout << "MyScore: " << thisScore << "\tEnemy score: " << enemyScore << std::endl;
}