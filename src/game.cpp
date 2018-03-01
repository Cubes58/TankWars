#include "game.h"


Game::Game() // Constructor
{
	// Set Backgound
	background.setSize(sf::Vector2f(780.0f,570.0f));
	background.setFillColor(sf::Color(40,70,20));
	background.setPosition(10.0f,10.0f);

	ammoArea.setSize(sf::Vector2f(800.0f,20.0f));
	ammoArea.setFillColor(sf::Color(140,90,60));
	ammoArea.setPosition(0.0f,580.0f);

	// Seed pseudorandom num gen
	srand ( (int) time(NULL) );

	// Set debug mode to off
	debugMode = false;

	// Borders
	obstacles.push_back(Obstacle(0.f,0.f,10.f,580.f,sf::Color(100,100,100)));
	obstacles.push_back(Obstacle(0.f,0.f,800.f,10.f,sf::Color(100,100,100)));
	obstacles.push_back(Obstacle(0.f,570.f,800.f,580.f,sf::Color(100,100,100)));
	obstacles.push_back(Obstacle(790.f,0.f,800.f,580.f,sf::Color(100,100,100)));

	float dx, dy;
	// Top right
	dx = (float) (rand() % 340 + 400);
	dy = (float) (rand() % 200 + 10);

	redBuildings.push_back(Obstacle(dx,dy,dx+20.f,dy+20.f,sf::Color(170,60,60)));
	redBuildings.push_back(Obstacle(dx+20.f,dy,dx+40.f,dy+20.f,sf::Color(170,40,40)));
	redBuildings.push_back(Obstacle(dx,dy+20.f,dx+20.f,dy+40.f,sf::Color(170,40,40)));
	redBuildings.push_back(Obstacle(dx+20.f,dy+20.f,dx+40.f,dy+40.f,sf::Color(170,60,60)));
	redBuildings.push_back(Obstacle(dx,dy+40.f,dx+20.f,dy+60.f,sf::Color(170,60,60)));
	redBuildings.push_back(Obstacle(dx+20.f,dy+40.f,dx+40.f,dy+60.f,sf::Color(170,40,40)));

	// Bottom right
	dx = (float) (rand() % 340 + 400);
	dy = (float) (rand() % 200 + 280);

	redBuildings.push_back(Obstacle(dx,dy,dx+20.f,dy+20.f,sf::Color(170,60,60)));
	redBuildings.push_back(Obstacle(dx+20.f,dy,dx+40.f,dy+20.f,sf::Color(170,40,40)));
	redBuildings.push_back(Obstacle(dx,dy+20.f,dx+20.f,dy+40.f,sf::Color(170,40,40)));
	redBuildings.push_back(Obstacle(dx+20.f,dy+20.f,dx+40.f,dy+40.f,sf::Color(170,60,60)));

	// Top left
	dx = (float) (rand() % 340 + 10);
	dy = (float) (rand() % 200 + 10);

	blueBuildings.push_back(Obstacle(dx,dy,dx+20,dy+20,sf::Color(60,60,170)));
	blueBuildings.push_back(Obstacle(dx+20,dy,dx+40,dy+20,sf::Color(40,40,170)));
	blueBuildings.push_back(Obstacle(dx,dy+20,dx+20,dy+40,sf::Color(40,40,170)));
	blueBuildings.push_back(Obstacle(dx+20,dy+20,dx+40,dy+40,sf::Color(60,60,170)));

	// Bottom left
	dx = (float) (rand() % 340 + 10);
	dy = (float) (rand() % 200 + 280);

	blueBuildings.push_back(Obstacle(dx,dy,dx+20,dy+20,sf::Color(60,60,170)));
	blueBuildings.push_back(Obstacle(dx+20,dy,dx+40,dy+20,sf::Color(40,40,170)));
	blueBuildings.push_back(Obstacle(dx,dy+20,dx+20,dy+40,sf::Color(40,40,170)));
	blueBuildings.push_back(Obstacle(dx+20,dy+20,dx+40,dy+40,sf::Color(60,60,170)));
	blueBuildings.push_back(Obstacle(dx,dy+40,dx+20,dy+60,sf::Color(60,60,170)));
	blueBuildings.push_back(Obstacle(dx+20,dy+40,dx+40,dy+60,sf::Color(40,40,170)));

	resetNpc();
	resetPlayer();

	redScore = 0;
	blueScore = 0;
	}

	Game::~Game(){}  // Destructor

	// Set a random Position which does not collide with anything
	void Game::resetNpc()
	{
	bool collision = true;
	while(collision)
	{
		float x = (float) (rand() % 360 + 400);
		float y = (float) (rand() % 580 + 10);
		float th = (float) (rand() % 359);
		float tth = th;
		npc.resetTank(x,y,th,tth);
		npc.reset();

		collision = false;
		for (list<Obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
		{
			if(npc.bb.collision(it->bb))
			{
				collision = true;
				break;
			}
		}
		for (list<Obstacle>::iterator it = blueBuildings.begin(); it != blueBuildings.end(); ++it)
		{
			if(npc.bb.collision(it->bb))
			{
				collision = true;
				break;
			}
		}
		for (list<Obstacle>::iterator it = redBuildings.begin(); it != redBuildings.end(); ++it)
		{
			if(npc.bb.collision(it->bb))
			{
				collision = true;
				break;
			}
		}
		if(npc.bb.collision(player.bb)) collision = true;
	}
}

// Set a random Position which does not collide with anything
void Game::resetPlayer()
{
	bool collision = true;
	while(collision)
	{
		float x = (float) (rand() % 380 + 10);
		float y = (float) (rand() % 580 + 10);
		float th =(float) ( rand() % 359);
		float tth = th;
		player.resetTank(x,y,th,tth);
		player.reset();

		collision = false;
		for (list<Obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
		{
			if(player.bb.collision(it->bb))
			{
				collision = true;
				break;
			}
		}
		for (list<Obstacle>::iterator it = blueBuildings.begin(); it != blueBuildings.end(); ++it)
		{
			if(player.bb.collision(it->bb))
			{
				collision = true;
				break;
			}
		}
		for (list<Obstacle>::iterator it = redBuildings.begin(); it != redBuildings.end(); ++it)
		{
			if(player.bb.collision(it->bb))
			{
				collision = true;
				break;
			}
		}
		if(player.bb.collision(npc.bb)) collision = true;
	}
}

void Game::play()// Play the game for one timestep
{
	// Move tank
	player.markPos();
	player.move();

	// Check for collisions
	bool collision = false;
	for (list<Obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
	{
		if(player.bb.collision(it->bb))
		{
			collision = true;
			break;
		}
	}
	for (list<Obstacle>::iterator it = blueBuildings.begin(); it != blueBuildings.end(); ++it)
	{
		if(player.bb.collision(it->bb))
		{
			collision = true;
			break;
		}
	}
	for (list<Obstacle>::iterator it = redBuildings.begin(); it != redBuildings.end(); ++it)
	{
		if(player.bb.collision(it->bb))
		{
			collision = true;
			break;
		}
	}
	if(player.bb.collision(npc.bb)) collision = true;

	if(collision)player.recallPos();


	// Move AI tank
	npc.markPos();
	npc.move();
	npc.implementMove();
	if(npc.isFiring()){fireShell(npc.firingPosition(), true);}

	// Check for collisions
	collision = false;
	for (list<Obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
	{
		if(npc.bb.collision(it->bb))
		{
			collision = true;
			break;
		}
	}
	for (list<Obstacle>::iterator it = blueBuildings.begin(); it != blueBuildings.end(); ++it)
	{
		if(npc.bb.collision(it->bb))
		{
			collision = true;
			break;
		}
	}
	for (list<Obstacle>::iterator it = redBuildings.begin(); it != redBuildings.end(); ++it)
	{
		if(npc.bb.collision(it->bb))
		{
			collision = true;
			break;
		}
	}
	if(npc.bb.collision(player.bb)) collision = true;

	if(collision)
	{
		npc.recallPos();
		npc.collided();
	}

	// Check if AI Tank can see anything
    for (list<Obstacle>::iterator it = redBuildings.begin(); it != redBuildings.end(); ++it)
    {
	  if(npc.canSee(it->bb)) npc.markBase(Position((it->bb.getX1() + it->bb.getX2()) / 2.0f, (it->bb.getY1() + it->bb.getY2()) / 2.0f));
    }
    for (list<Obstacle>::iterator it = blueBuildings.begin(); it != blueBuildings.end(); ++it)
    {
	  if(npc.canSee(it->bb)) npc.markTarget(Position((it->bb.getX1() + it->bb.getX2()) / 2.0f, (it->bb.getY1() + it->bb.getY2()) / 2.0f));
    }
    for (list<Shell>::iterator it = shells.begin(); it != shells.end(); ++it)
    {
	  if(npc.canSee(it->bb) && !it->isNpc()) npc.markShell(Position((it->bb.getX1() + it->bb.getX2()) / 2.0f, (it->bb.getY1() + it->bb.getY2()) / 2.0f));
    }
	if(npc.canSee(player.bb)) npc.markEnemy(Position((player.bb.getX1() + player.bb.getX2()) / 2.0f, (player.bb.getY1() + player.bb.getY2()) / 2.0f ));

	// Move shells
	for (list<Shell>::iterator it = shells.begin(); it != shells.end(); ++it){it->move();}

	// Check if shells have hit anything

	// Have shells hit edges
	for (list<Obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
	{
		for (list<Shell>::iterator it2 = shells.begin(); it2 != shells.end(); it2++)
	    {
		  if(it->bb.collision(it2->bb))
		  {
			shells.erase(it2);
			break;
		  }
		}
	}
	// Second check
	for (list<Shell>::iterator it2 = shells.begin(); it2 != shells.end(); it2++)
	{
		if(fabs(it2->getY()) > 1000)
		{
		shells.erase(it2);
		break;
		}
		if(fabs(it2->getX()) > 1200)
		{
		shells.erase(it2);
		break;
		}
	}

	// Have shells hit red buildings
	bool stop = false;
	list<Obstacle>::iterator it = redBuildings.begin();
	while (!(stop || it == redBuildings.end()))
	{
		
		list<Shell>::iterator it2 = shells.begin();
		while(!(stop || it2 == shells.end()))
		{
			if(it->bb.collision(it2->bb) && (it2->couldSeeWhenFired(it->bb) || it->isVisible()) )
			{
				shells.erase(it2);
				redBuildings.erase(it);
				blueScore += 10;
				npc.score(redScore, blueScore);
				stop = true;
			}
			else it2++;
		}
		
		if(!stop)++it;
	}

	// Have shells hit blue buildings
	stop = false;
	it = blueBuildings.begin();
	while (!(stop || it == blueBuildings.end()))
	{

		list<Shell>::iterator it2 = shells.begin();
		while(!(stop || it2 == shells.end()))
		{
			if(it->bb.collision(it2->bb) && (it2->couldSeeWhenFired(it->bb) || it->isVisible()))
			{
				shells.erase(it2);
				blueBuildings.erase(it);
				redScore += 10;
				npc.score(redScore, blueScore);
				stop = true;
			}
			else it2++;
		}

		if(!stop)++it;
	}

	// Have shells hit red tank
	stop = false;
	list<Shell>::iterator it2 = shells.begin();
	while(!(stop || it2 == shells.end()))
	{
		if(it2->couldSeeWhenFired(npc.bb) && npc.bb.collision(it2->bb))
		{
			shells.erase(it2);
			resetNpc();
			blueScore += 25;
			npc.score(redScore, blueScore);
			stop = true;
		}
		else it2++;
	}


	// Have shells hit BLUE tank
	stop = false;

	it2 = shells.begin();
	while(!(stop || it2 == shells.end()))
	{
		if(it2->couldSeeWhenFired(player.bb) && player.bb.collision(it2->bb))
		{
			shells.erase(it2);
			resetPlayer();
			redScore += 25;
			npc.score(redScore, blueScore);
			stop = true;
		}
		else it2++;
	}

	for (list<Obstacle>::iterator it = redBuildings.begin(); it != redBuildings.end(); ++it)
	{
		if(player.canSee(it->bb)) it->setVisible();
	}

	for (list<Obstacle>::iterator it = blueBuildings.begin(); it != blueBuildings.end(); ++it)
	{
		if(player.canSee(it->bb)) it->setVisible();
	}
	if(player.canSee(npc.bb)) {npc.setVisible();}
	else {npc.setInvisible();}

	for (list<Shell>::iterator it = shells.begin(); it != shells.end(); ++it)
	{
		if(player.canSee(it->bb)) it->setVisible();
	}

}

void Game::fireShell(Position fp, bool isNpc)
{
   Position shellPos = fp;
   Shell newShell(fp, isNpc);
   bool ai = false;
   bool tank = false;
   bool canFire = false;
   // Sort out visibility
   if(isNpc)
   {
		if(npc.canFire())
		{
			npc.fireShell();
			canFire = true;
		}
   }
   else
   {
	    if(player.canFire())
		{
			player.fireShell();
			canFire = true;
		}
   }

   if(canFire)shells.push_back(newShell);
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const// Draw the game
{
	target.draw(background);

	target.draw(ammoArea);

	// Draw shells
	for (list<Shell>::const_iterator it = shells.begin(); it != shells.end(); ++it)
	{
		if(it->isVisible() || debugMode)  target.draw(*it);
	}

	// Draw obstacles
	for (list<Obstacle>::const_iterator it = obstacles.begin(); it != obstacles.end(); ++it) 
	{
		target.draw(*it);
	}

	// Draw Red Buildings
	for (list<Obstacle>::const_iterator it = redBuildings.begin(); it != redBuildings.end(); ++it)
	{
		if(it->isVisible() || debugMode) target.draw(*it);
	}

	// Draw blue buildings
	for (list<Obstacle>::const_iterator it = blueBuildings.begin(); it != blueBuildings.end(); ++it)
	{
		if(it->isVisible()  || debugMode) target.draw(*it);
	}

	// Draw AITank
	if (npc.isVisible() || debugMode) target.draw(npc);

	// Draw Player
	target.draw(player);
	
	// Draw ammo
	sf::RectangleShape ammo(sf::Vector2f(5,10));
	ammo.setFillColor(sf::Color(0,0,255));
	for(int i=0;i<player.getNumberOfShells();i++)
	{
		ammo.setPosition((float)i*15+10, 585.f);
		target.draw(ammo);
	}

	ammo.setFillColor(sf::Color(255,0,0));
	for(int i=0;i<npc.getNumberOfShells();i++)
	{
		ammo.setPosition((float)790-i*15, 585.f);
		target.draw(ammo);
	}

	// Draw scores
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	char msg[255];
	sprintf_s(msg,"%d", blueScore);
	sf::Text drawingText(sf::String(msg),font,18);
	drawingText.setColor(sf::Color::White);

	drawingText.setPosition(250,577);
	target.draw(drawingText);

	sprintf_s(msg,"%d", redScore);
	drawingText.setString(sf::String(msg));
	drawingText.setPosition(550,577);
	target.draw(drawingText);

	// Draw game over
	if(gameOver())
	{
		sprintf_s(msg,"GAME OVER", blueScore);
		sf::Text drawingText(sf::String(msg),font,42);
		drawingText.setPosition(300, 140);
		target.draw(drawingText);
		drawingText.setPosition(300, 240);
		if(redScore > blueScore)
		{
			sprintf_s(msg,"RED WINS!", blueScore);
			drawingText.setString(sf::String(msg));
		}
		if(redScore < blueScore)
		{
			sprintf_s(msg,"BLUE WINS!", blueScore);
			drawingText.setString(sf::String(msg));
		}
		if(redScore == blueScore)
		{
			sprintf_s(msg,"MATCH DRAW!", blueScore);
			drawingText.setString(sf::String(msg));
		}
		target.draw(drawingText);
	}
}
     
void Game::keyPressed(sf::Keyboard::Key key)
{
	switch(key)
	{
	   case	sf::Keyboard::Tab :
		   debugMode = !debugMode;
		   player.toggleDebugMode();
		   npc.toggleDebugMode();
		   for (list<Shell>::iterator it = shells.begin(); it != shells.end(); ++it){it->toggleDebugMode();}
		   break;
	   case  sf::Keyboard::W : 
		   player.goForward();
		   break;
	   case  sf::Keyboard::A : 
		   player.goLeft();
		   break;
	   case  sf::Keyboard::S : 
		   player.goBackward();
		   break;
	   case  sf::Keyboard::D : 
		   player.goRight();
		   break;
	   case	sf::Keyboard::Space :
		   if(player.canFire())
		   {
			player.fire();
			fireShell(player.firingPosition(), false);
		   }
		   break;
	   case  sf::Keyboard::Left:
		   player.turretGoLeft();
		   break;
	   case  sf::Keyboard::Right:
		   player.turretGoRight();
		   break;
	}
}

void Game::keyReleased(sf::Keyboard::Key key)
{
	switch(key)
	{
	   case  sf::Keyboard::W : 
		   player.stop();
		   break;
	   case  sf::Keyboard::A : 
		   player.stop();
		   break;
	   case  sf::Keyboard::S : 
		   player.stop();
		   break;
	   case  sf::Keyboard::D : 
		   player.stop();
		   break;
	   case  sf::Keyboard::Left:
		   player.stopTurret();
		   break;
	   case  sf::Keyboard::Right:
		   player.stopTurret();
		   break;
	}
}

bool Game::gameOver() const
{
	return numBlueBuildings() == 0 || numRedBuildings() == 0 || (!(player.hasAmmo() || npc.hasAmmo()) && shells.empty());
}

int Game::numBlueBuildings() const
{
	return blueBuildings.size();
}

int Game::numRedBuildings() const
{
	return redBuildings.size();
}