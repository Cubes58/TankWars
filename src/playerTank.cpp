#include "PlayerTank.h"
#include <iostream>
PlayerTank::PlayerTank() // Construtor
{
	clearMovement();
	turretLeft = false;
	turretRight = false;
	numberOfShells = 15;

	bodyTex.loadFromFile("assets\\blueTank.png");
	body.setTexture(bodyTex);
	body.setOrigin(100,100);
	body.setScale(0.2f,0.2f);

	turretTex.loadFromFile("assets\\blueTankTurret.png");
	turret.setTexture(turretTex);
	turret.setOrigin(46,44);
	turret.setScale(0.2f,0.2f);
}

void PlayerTank::move()
{	
	implementMove();
}

void PlayerTank::fire()
{
	
}