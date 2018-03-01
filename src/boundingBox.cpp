#include "boundingBox.h"

BoundingBox::BoundingBox() // Construtor
{
}

BoundingBox::~BoundingBox(){} // Destructor

void BoundingBox::draw(sf::RenderTarget &target, sf::RenderStates states) const // Draw BB
{
	sf::VertexArray box(sf::LinesStrip,5);
	box[0] = sf::Vertex(sf::Vector2f(x1,y1),sf::Color::White);
	box[1] = sf::Vertex(sf::Vector2f(x1,y2),sf::Color::White);
	box[2] = sf::Vertex(sf::Vector2f(x2,y2),sf::Color::White);
	box[3] = sf::Vertex(sf::Vector2f(x2,y1),sf::Color::White);
	box[4] = sf::Vertex(sf::Vector2f(x1,y1),sf::Color::White);
	target.draw(box);
	/*drawing.drawLine(x1,y1,x2,y1,3,sf::Color(255,0,0));
	drawing.drawLine(x1,y1,x1,y2,3,sf::Color(255,0,0));
	drawing.drawLine(x1,y2,x2,y2,3,sf::Color(255,0,0));
	drawing.drawLine(x2,y1,x2,y2,3,sf::Color(255,0,0));*/
}

void BoundingBox::set(float xa, float ya, float xb, float yb)
{
	x1 = xa;
	y1 = ya;
	x2 = xb;
	y2 = yb;
}

bool BoundingBox::collision(BoundingBox otherBb) const
{
	bool c1 = x2 < otherBb.getX1();
	bool c2 = otherBb.getX2() < x1;
	bool c3 = y2 < otherBb.getY1();
	bool c4 = otherBb.getY2() < y1;

	return !(c1 || c2 || c3 || c4);
}

bool BoundingBox::lineCollision(float x1l, float y1l, float x2l, float y2l) const
{
	BoundingBox lineBb;
	lineBb.set(std::min(x1l,x2l),std::min(y1l,y2l),std::max(x1l,x2l),std::max(y1l,y2l));
	if(!collision(lineBb)) return false;

	float dx, dy, m, c; // Line calc
	dx = x2l - x1l;
	dy = y2l - y1l;
	m = dy / dx;
	c = y1l - m * x1l;

	// Test if all corners are above or below
	bool tlabove, trabove, blabove, brabove;
	float y;

	y = m * x1 + c;
	tlabove = y > y1;
	blabove = y > y2;
	y = m * x2 + c;
	trabove = y > y1;
	brabove = y > y2;

	return !(tlabove == trabove && tlabove == blabove && tlabove == brabove);
}