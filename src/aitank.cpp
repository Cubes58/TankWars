#include "AITank.h"

AITank::AITank() // Construtor
{
	bodyTex.loadFromFile("assets\\redTank.png");
	body.setTexture(bodyTex);
	body.setOrigin(100,100);
	body.setScale(0.2f,0.2f);

	turretTex.loadFromFile("assets\\redTankTurret.png");
	turret.setTexture(turretTex);
	turret.setOrigin(46,44);
	turret.setScale(0.2f,0.2f);
	
	numberOfShells = 15;
}










