#include <SFML/Graphics.hpp>
#include "game.h"

int main()
{


    // Create main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tankwar");
 
	// Create a game
	Game game;
	
    // Create a clock for measuring the time elapsed
    sf::Clock clock1;
	float delay = 0.0f;

    // Start game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
			if (event.type == sf::Event::Closed) 
			{window.close();}
           
			// Key press
			if (event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Escape)// Escape key : exit
				{window.close();}
				else // Pass key press to game
				{game.keyPressed(event.key.code);}
			}
			// Key released
			if (event.type == sf::Event::KeyReleased)
			{
				game.keyReleased(event.key.code);
			}
		}
			
		
		// Run at 40 frame a second
		delay=clock1.getElapsedTime().asSeconds();
		if(delay>0.025 && !game.gameOver()) {
			game.play();
			clock1.restart();
		}

		// Clear the window
	    window.clear(sf::Color::Black);

		//Draw the game
		window.draw(game);

        // Finally, display the rendered frame on screen
        window.display();
    }
  

    return EXIT_SUCCESS;

}
