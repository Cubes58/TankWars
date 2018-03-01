#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include "tank.h"

class PlayerTank : public Tank
{
public:
      PlayerTank(); // Construtor

	  void move(); // Move tank
	  void fire(); // Fire shell
	  void reset(){}; // Reset variables
};
#endif