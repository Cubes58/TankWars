#include "shell.h"

Shell::Shell(Position startPos, bool isNPC) // Constructor
{
	pos = startPos;
	firingPosition = pos;

	npc = isNPC;

	float x,y;
	x = pos.getX();
	y = pos.getY();

	box.setFillColor(sf::Color(90,90,90));
	box.setSize(sf::Vector2f(6.0f,12.0f));
	box.setOrigin(2,6);

	updateBb();
	debugMode = false;
	visible = false;
}

void Shell::move() // Move Shell
{	
	float x,y,th;
	x = pos.getX();
	y = pos.getY();
	th = pos.getTh();
	float thRad = DEG2RAD(th); // Heading in radians

	float dx = cos(thRad) * shellMoveConst;
	float dy = sin(thRad) * shellMoveConst;
	pos.set(x + dx, y + dy, th);

	updateBb();
}

void Shell::updateBb()
{
	float x,y,th;
	x = pos.getX();
	y = pos.getY();
	th = pos.getTh()-90.0f;
	bb.set(x-7.0f,y-7.0f,x+7.0f,y+7.0f);

	box.setPosition(x,y);
	box.setRotation(th);
}

void Shell::draw(sf::RenderTarget &target, sf::RenderStates states) const // Draw Shell
{
	target.draw(box);
	if(debugMode)target.draw(bb);
}

bool Shell::couldSeeWhenFired(BoundingBox object)
{
	float dx = object.getXc() - firingPosition.getX();
	float dy =  object.getYc() - firingPosition.getY();
	float dist = sqrt( dx * dx + dy * dy );

	if(dist > 250.0f) return false;
	if(dist < 120.0f) return true;
 
	float angle = atan2(dy, dx);
	if(angle > PI){angle = angle - 2.0f * PI;}
	if(angle < -PI){angle = angle + 2.0f * PI;}
	float thRad = DEG2RAD(firingPosition.getTh());
	if(thRad > PI){thRad = thRad - 2.0f * PI;}
	if(thRad < -PI){thRad = thRad + 2.0f * PI;}
	float diff = angle - thRad;
	if(diff > PI){diff = diff - 2.0f * PI;}
	if(diff < -PI){diff = diff + 2.0f * PI;}

	return fabs(diff) < 0.4f;
}
