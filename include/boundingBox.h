#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <SFML/Graphics.hpp>

class BoundingBox  : public sf::Drawable
{
private:
	  float x1,x2,y1,y2;
public:
      BoundingBox(); //!< Empty construtor
      ~BoundingBox(); // Destructor
      virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const; // Draw BB
	  void set(float xa, float ya, float xb, float yb);
	  float getX1() const {return x1;}
	  float getY1() const {return y1;}
	  float getX2() const {return x2;}
	  float getY2() const {return y2;}
	  float getXc() const {return (x1 + x2) / 2.0f;}
	  float getYc() const {return (y1 + y2) / 2.0f;}
	  bool collision(BoundingBox otherBb) const;
	  bool lineCollision(float x1, float y1, float x2, float y2) const;
};
#endif