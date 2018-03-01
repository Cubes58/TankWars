#include "obstacle.h"

Obstacle::Obstacle() // Construtor
{
	visible = false;
	debugMode = false;
}

Obstacle::Obstacle(float x1, float y1, float x2, float y2, sf::Color c) // Construtor
{
	bb.set(x1,y1,x2,y2);

	block.setSize(sf::Vector2f(x2-x1,y2-y1));
	block.setFillColor(c);
	block.setPosition(x1,y1);

	visible = false;
	debugMode = false;
}

Obstacle::~Obstacle(){} // Destructor

void Obstacle::pointDist()
{
	float d1, d2, d3, d4;
	float x1, y1, x2, y2;

	x1 = bb.getX1();
	y1 = bb.getY1();
	x2 = bb.getX2();
	y2 = bb.getY2();

	d1 = sqrt( (x1 - xComp) * (x1 - xComp) + (y1 - yComp) * (y1 - yComp) );
	d2 = sqrt( (x2 - xComp) * (x2 - xComp) + (y1 - yComp) * (y1 - yComp) );
	d3 = sqrt( (x1 - xComp) * (x1 - xComp) + (y2 - yComp) * (y2 - yComp) );
	d4 = sqrt( (x2 - xComp) * (x2 - xComp) + (y2 - yComp) * (y2 - yComp) );

	dist = std::min(std::min(std::min(d1,d2),d3),d4) ;
}

void Obstacle::draw(sf::RenderTarget &target, sf::RenderStates states) const // Draw
{
	target.draw(block);
	if(debugMode) target.draw(bb);
}

