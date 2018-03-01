#ifndef TANK_H
#define TANK_H


#include <SFML/Graphics.hpp>

#include "position.h"
#include "boundingBox.h"

class Tank : public sf::Drawable
{
private:
		short fireCounter; //!< Game tick counter for firing
		static const float moveConst; //!< Total amount of movement allowed each timestep
		static const float rotMoveConst; //!< Total amount of rotational movement allowed each timestep for the tank
		static const float turRotMoveConst; //!< Total amount of rotational movement allowed each timestep for the turrent
protected:
		Position pos; //!< Current position of the tank
		Position oldPos; //!< Previous position of the tank, used in very basic position correction for collisions
		float turretTh; //!< Current heading of the gun turret
		int numberOfShells; //!< Amount of ammo left

		bool forward; //!< Tank moving forwards
		bool backward; //!< Tank moving backwards
		bool left; //!< Tank turning left
		bool right; //!< Tnak turning right
		bool turretLeft; //!< Turret turning left
		bool turretRight; //!< Turret turing right

		void clearMovement(); //!< Stop current movement
		void updateBb(); //!< Update the bounding box position to be the same as the tank
		sf::Sprite body; //!< Sprite used to draw the tank
		sf::Texture bodyTex; //!< Texture used by the tank sprite
		sf::Sprite turret; //!< Sprite used to draw the tanks turret 
		sf::Texture turretTex; //!< Texture used by the tank turret sprite 
		bool debugMode; //!< Debug mode - show more stuff
public:
		Tank();
		BoundingBox bb; //!< BB for collision detection
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const; //!< Draw Tank
		void resetTank(float newX, float newY, float newTh, float newTurretTh); //!< Reset Tank's position
			// Movement methods
		void goForward(); //!< Set the tank to go forwards
		void goBackward(); //!< Set the tank to go backwards
		void goLeft(); //!< Set the tank to go left
		void goRight(); //!< Set the tank to go right
		void turretGoLeft(); //!< Set the turret to go left
		void turretGoRight(); //!< Set the turret to go right
		void stop(){clearMovement();} //!< Stop the tank body movement
		void stopTurret(); //!< Stop the turret movement
		void markPos(){oldPos = pos;} //!< Record the prevoius position
		void recallPos(){pos = oldPos;updateBb();} //!< Go back to the prevoius position - simple form of position correction

		void fireShell(); //!< Fire a shell
		virtual void move() = 0; //?!< Implemented in child classes
		void implementMove(); //!< Move tank, relise what is in the move funtion

		Position firingPosition() const; //!< Position of the tank as shell is fired
		float getX() const{return pos.getX();} //!< Position of the tank in x
		float getY() const{return pos.getY();} //!< Position of the tank in y
		int getNumberOfShells()const{return numberOfShells;} //!< Amount of ammo left
		bool canSee(BoundingBox other) const; //!< Can this tank see the bounding box?
		bool canFire() const {return numberOfShells > 0 && fireCounter == 0;} //!< Can this tnak fire
		bool hasAmmo() const {return numberOfShells > 0;} //!< Does this tank have nay ammo left
		void toggleDebugMode(){debugMode = !debugMode;} //!< Toggle the debug mode
};
#endif