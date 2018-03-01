#ifndef position_H
#define position_H

#define PI 3.1415927f
#define DEG2RAD(DEG) ((DEG)*((PI)/(180.0f)))
#define RAD2DEG(RAD) ((RAD)*((180.0f)/(PI)))

class Position
{
private:
	  float x,y,th;
   public:
	   Position(){}; // Construtor
	   Position(float newX, float newY){x = newX; y = newY;} // Constructor with params
	   ~Position(){}; // Destructor
	  void set(float newX, float newY, float newTh){x = newX;y = newY;th = newTh;}
	  float getX() const {return x;}
	  float getY() const {return y;}
	  float getTh() const {return th;}
};
#endif