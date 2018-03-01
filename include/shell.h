#ifndef SHELL_H
#define SHELL_H

#include <SFML/Graphics.hpp>

#include "Position.h"
#include "boundingBox.h"

#define shellMoveConst 3

class Shell : public sf::Drawable
{
protected:
	Position pos;
	Position firingPosition;
	void updateBb();
	sf::RectangleShape box;
	bool debugMode;
	bool visible;
	bool npc;
public:
	Shell(Position pos, bool isNPC);
	BoundingBox bb; // BB for collision detection
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;// Draw Shell
	void move(); // Move Shell
	float getX(){return pos.getX();}
	float getY(){return pos.getY();}
	void toggleDebugMode(){debugMode = !debugMode;}
	void setVisible(){visible = true;}
	bool isVisible()const {return visible;}
	bool isNpc()const {return npc;}
	bool couldSeeWhenFired(BoundingBox object);
};
#endif