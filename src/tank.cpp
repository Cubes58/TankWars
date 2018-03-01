#include "Tank.h"
#include <iostream>


Tank::Tank()
{
	fireCounter = 0;
	debugMode = false;
}

const float Tank::moveConst = 1.75f; // Total amount of movement allowed each timestep
const float Tank::rotMoveConst = 1.25f; // Total amount of rotational movement allowed each timestep for the tank
const float Tank::turRotMoveConst = 0.75f; // Total amount of rotational movement allowed each timestep for the turrent

void Tank::resetTank(float newX, float newY, float newTh, float newTurretTh)
{
	pos.set(newX,newY,newTh);
	turretTh = newTurretTh;

	body.setPosition(newX,newY);
	body.setRotation(newTh);

	turret.setPosition(newX,newY);
	turret.setRotation(newTh);

	updateBb();
}

void Tank::fireShell()
{
	if(canFire())
	{
		// Set fire counter
		fireCounter = 80;
		// Decrement ammo
		numberOfShells--;
	}
}

// Can the tank see the bounding box
bool Tank::canSee(BoundingBox other) const
{

	float dx = other.getXc() - bb.getXc();
	float dy = other.getYc() - bb.getYc();
	float dist = sqrt( dx * dx + dy * dy );

	if(dist > 250.0f) return false;
	if(dist < 120.0f) return true;

	float angle = atan2(dy, dx);
	
	float thRad = DEG2RAD(turretTh);
	if(thRad > PI){thRad = thRad - 2.0f * PI;}
	float diff = angle - thRad;

	return fabs(diff) < 0.4f;
}

// Position where the shell leaves the tank
Position Tank::firingPosition() const
{
	Position fp;

	float thRad = DEG2RAD(turretTh);
	float fireX = cos(thRad) * 40.0f + pos.getX();
	float fireY = sin(thRad) * 40.0f + pos.getY();

	fp.set(fireX, fireY, turretTh);
	return fp;
}

void Tank::updateBb()
{
	float x,y;
	x = pos.getX();
	y = pos.getY();
	bb.set(x-20.0f,y-20.0f,x+20.0f,y+20.0f);
}

void Tank::implementMove()
{
	float x,y,th;
	x = pos.getX();
	y = pos.getY();
	th = pos.getTh();
	float thRad = DEG2RAD(th); // Heading in radians
	float dx = cos(thRad) * moveConst;
	float dy = sin(thRad) * moveConst;

	if(forward)
	{
		pos.set(x + dx, y + dy, th);
	}

	if(backward)
	{
		pos.set(x - dx, y - dy, th);
	}

    if(left)
	{
		float newTh = th-rotMoveConst;
		if(newTh < 0){newTh += 360.0;}
		pos.set(x, y, newTh);

		newTh = turretTh-rotMoveConst;
		if(newTh < 0){newTh += 360.0;}
		turretTh = newTh;
	}

	if(right)
	{
		float newTh = th+rotMoveConst;
		if(newTh > 360.0){newTh -= 360.0;}
		pos.set(x, y, newTh);

		newTh = turretTh+rotMoveConst;
		if(newTh > 360.0){newTh -= 360.0;}
		turretTh = newTh;
	}

	if(turretLeft)
	{
		float newTh = turretTh - turRotMoveConst;
		if(newTh < 0.0){newTh += 360.0;}
		turretTh = newTh;
	}

	if(turretRight)
	{
		float newTh = turretTh + turRotMoveConst;
		if(newTh > 360.0){newTh -= 360.0;}
		turretTh = newTh;
	}

	body.setPosition(pos.getX(),pos.getY());
	body.setRotation(pos.getTh());

	turret.setPosition(pos.getX(),pos.getY());
	turret.setRotation(turretTh);

	updateBb(); // Update the bounding box
	// Decrement fire counter
	if(fireCounter > 0) fireCounter--;
}

void Tank::clearMovement()
{
	forward = false;
	backward = false;
	left = false;
	right = false;
}

void Tank::goForward()
{
	clearMovement();
	forward = true;
}

void Tank::goBackward()
{
	clearMovement();
	backward = true;
}

void Tank::goLeft()
{
	clearMovement();
	left = true;
}

void Tank::goRight()
{
	clearMovement();
	right = true;
}

void Tank::turretGoLeft()
{
	turretLeft = true;
	turretRight = false;
}

void Tank::turretGoRight()
{
	turretLeft = false;
	turretRight = true;
}

void Tank::stopTurret()
{
	turretLeft = false;
	turretRight = false;
}

void Tank::draw(sf::RenderTarget &target, sf::RenderStates states) const // Draw Tank
{
	target.draw(body);
	target.draw(turret);
	if(debugMode)target.draw(bb);
	/*glPushMatrix();
	glTranslatef(pos.getX(),pos.getY(),0);
	glRotatef(pos.getTh()-90.0,0.0,0.0,1.0);

	// Main body
	glColor3f((float)colour.r / 255.0, (float)colour.g / 255.0, (float)colour.b / 255.0);
	glBegin(GL_QUADS);
	glVertex2d(-16,24);
	glVertex2d(16,24);
	glVertex2d(16,-24);
	glVertex2d(-16,-24);
	glEnd();

	// Left track
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex2d(-24,22);
	glVertex2d(-17,22);
	glVertex2d(-17,-22);
	glVertex2d(-24,-22);
	glEnd();

	// Right track
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex2d(24,22);
	glVertex2d(17,22);
	glVertex2d(17,-22);
	glVertex2d(24,-22);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(pos.getX(),pos.getY(),0);
	glRotatef(turretTh-90.0,0.0,0.0,1.0);
	// Turret
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex2d(-4,10);
	glVertex2d(4,10);
	glVertex2d(4,50);
	glVertex2d(-4,50);
	glEnd();

	glPopMatrix();

	drawing.drawArc(pos.getX(),pos.getY(),10.0,0,6.28,60,3.0,sf::Color(0,0,0));

	if(debugMode)bb.draw();*/
}




