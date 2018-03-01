/** \file aitank,.h **/
#ifndef AITANK_H
#define AITANK_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "tank.h"
#include "position.h"

/** \class AITANK \brief Simple AI Tank**/
class AITank : public Tank
{
protected:
	bool visible;
public:
	AITank(); //!< Empty construtor

	void setVisible(){visible = true;} //!< Make the AI tank visible to the player
	void setInvisible(){visible = false;} //!< Make the AI tank invisible to the player
	bool isVisible()const {return visible;} //!< Is the tank visiable to the player?

	// FSM public methods, set states and variable
	virtual void reset() = 0; //!< Reset any variables you need to whent he tank has been shot
	virtual void move() = 0; //!< Move the tank
	virtual void collided() = 0; //!< Called by the game object when the tank has collided
	virtual void markTarget(Position p /** \param Position of the acquired target */) = 0; //!< Called by the game object when a target (enemy building) comes within the tanks visible range
	virtual void markEnemy(Position p /** \param Position of the enemy tank */) = 0; //!< Called by the game object when the enemy tank comes within the tanks visible range
	virtual void markBase(Position p /** \param Position of the home building */) = 0; //!< Called by the game object when one of you own buildings comes within the tanks visible range
	virtual void markShell(Position p /** \param Position of an enemy shell */) = 0; //!< Called by the game object when enemy shell comes within the tanks visible range
	virtual bool isFiring() = 0; //!< Called by the game object each frame.  When this function returns true (and ammo is availbe and loaded) the tank will fire a shell
	virtual void score(int thisScore,int enemyScore) = 0;
};

#endif