#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "boundingBox.h"

class Obstacle : public sf::Drawable
{
private:
	  sf::RectangleShape block;
	  float xComp, yComp;
	  void pointDist();
	  bool visible;
	  bool debugMode;
public:
      Obstacle(); // Construtor
	  Obstacle(float x1, float y1, float x2, float y2, sf::Color c); // Construtor
      ~Obstacle(); // Destructor
      virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const; // Draw obstacle
	  BoundingBox bb;
	  void setPoint(float x, float y){xComp = x;yComp = y;pointDist();}
	  bool operator<(const Obstacle &other) {return dist < other.dist;}
	  float dist;
	  void setVisible(){visible = true;}
	  bool isVisible()const {return visible;}
	  void toggleDebugMode(){debugMode = !debugMode;}
};
#endif