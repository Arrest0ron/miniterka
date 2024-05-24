#pragma once

#include <SFML/Graphics.hpp>
extern const int MAP_LENGTH;
extern int tileSize;
extern const float  MOVEMENTCAP;


void ViewDynamics(sf::View& UserView)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && UserView.getSize().x < MAP_LENGTH*tileSize*2)
	{
		UserView.zoom(1.02);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && UserView.getSize().x > MAP_LENGTH*tileSize/100)
	{
		UserView.zoom(0.98);
	}
}

bool IsGreaterThenLimitX(sf::Vector2f& movement)
{
    if (pow(movement.x,2) >= MOVEMENTCAP)
    {
        return 1;
    }
    return 0;
}

void MovementCalculation(const float BaseSpeed,Update& upd,Player& User)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && ( (!IsGreaterThenLimitX(User.movement)) || User.movement.x > 0 ))
	{
		User.movement.x -= BaseSpeed/3 + BaseSpeed/3*2*(abs(User.movement.x)/MOVEMENTCAP);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)&& ( (!IsGreaterThenLimitX(User.movement)) || User.movement.x < 0 ))
	{
		User.movement.x += BaseSpeed/3 + BaseSpeed/3*2*(abs(User.movement.x)/MOVEMENTCAP);

	}

	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (User.GetCollision()[1] == 1))
	{
		User.movement.y-=(BaseSpeed+0.05)*16;
	}

	// Остановка при крайне маленьком движении.
	if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::D))&&(!sf::Keyboard::isKeyPressed(sf::Keyboard::A))) 
	{

		User.movement.x /= (1+BaseSpeed*0.2);
		if ((-0.01 < User.movement.x) && (User.movement.x< 0.01 ))
		{
			User.movement.x = 0;
		}
	}
	if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::W))&&(!sf::Keyboard::isKeyPressed(sf::Keyboard::S))) 
	{
		User.movement.y /= (1+BaseSpeed*0.2);
		if ((-0.01 < User.movement.y) && (User.movement.y< 0.01 ))
		{
			User.movement.y = 0;
		}
	}
		//Движение с учетом коллизии
	for (int i =0 ; i != 16; i++)
	{
		upd.UpdatePlayer(User);
		std::vector<int> calledColl = User.GetCollision();
		if ((calledColl[0] == 1) && (User.movement.y<0))
		{
			User.movement.y=-0;
		}
		if ((calledColl[1] == 1) && (User.movement.y>0))
		{
			User.movement.y=0;
		}
		if ((calledColl[2] == 1) && (User.movement.x<0))
		{
			User.movement.x=0;
		}
		if ((calledColl[3] == 1) && (User.movement.x>0))
		{
			User.movement.x=-0;
		}
		if ((calledColl[0] == 2))
		{
			User.movement.y=-User.movement.y/3 -0.1;
		}
		if ((calledColl[1] == 2) )
		{
			User.movement.y=-User.movement.y/3 + 0.1;
		}
		if ((calledColl[2] == 2) )
		{
			User.movement.x=-User.movement.x/3 + 0.1;
		}
		if ((calledColl[3] == 2) )
		{
			User.movement.x=-User.movement.x/3 -0.1;
		}
		
		User.GetSprite().move(User.movement.x/16,User.movement.y/16);

	}
}

