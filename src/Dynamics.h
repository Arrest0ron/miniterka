#pragma once

#include <SFML/Graphics.hpp>


#include <cmath>
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
    if (std::pow(movement.x,2) >= MOVEMENTCAP)
    {
        return 1;
    }
    return 0;
}

void MovementCalculation(const float BaseSpeed,Update& upd,Entity& ent)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && ( (!IsGreaterThenLimitX(ent.movement)) || ent.movement.x > 0 ))
	{
		ent.movement.x -= BaseSpeed/3 + BaseSpeed/3*2*(abs(ent.movement.x)/MOVEMENTCAP);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)&& ( (!IsGreaterThenLimitX(ent.movement)) || ent.movement.x < 0 ))
	{
		ent.movement.x += BaseSpeed/3 + BaseSpeed/3*2*(abs(ent.movement.x)/MOVEMENTCAP);

	}

	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (ent.GetCollision()[1] == 1))
	{
		ent.movement.y-=(BaseSpeed+0.05)*16;
	}

	// Остановка при крайне маленьком движении.
	if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::D))&&(!sf::Keyboard::isKeyPressed(sf::Keyboard::A))) 
	{

		ent.movement.x /= (1+BaseSpeed*0.2);
		if ((-0.01 < ent.movement.x) && (ent.movement.x< 0.01 ))
		{
			ent.movement.x = 0;
		}
	}
	if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::W))&&(!sf::Keyboard::isKeyPressed(sf::Keyboard::S))) 
	{
		ent.movement.y /= (1+BaseSpeed*0.2);
		if ((-0.01 < ent.movement.y) && (ent.movement.y< 0.01 ))
		{
			ent.movement.y = 0;
		}
	}
}

void CollisionDynamic(const float BaseSpeed,Update& upd,Entity& ent)
{
		//Движение с учетом коллизии
	for (int i =0 ; i != 16; i++)
	{
		upd.Collision(ent);
		std::vector<int> calledColl = ent.GetCollision();
		if ((calledColl[0] == 1) && (ent.movement.y<0))
		{
			ent.movement.y=-0;
		}
		if ((calledColl[1] == 1) && (ent.movement.y>0))
		{
			ent.movement.y=0;
		}
		if ((calledColl[2] == 1) && (ent.movement.x<0))
		{
			ent.movement.x=0;
		}
		if ((calledColl[3] == 1) && (ent.movement.x>0))
		{
			ent.movement.x=-0;
		}
		if ((calledColl[0] == 2))
		{
			ent.movement.y=-ent.movement.y/3 -0.1;
		}
		if ((calledColl[1] == 2) )
		{
			ent.movement.y=-ent.movement.y/3 + 0.1;
		}
		if ((calledColl[2] == 2) )
		{
			ent.movement.x=-ent.movement.x/3 + 0.1;
		}
		if ((calledColl[3] == 2) )
		{
			ent.movement.x=-ent.movement.x/3 -0.1;
		}
		
		ent.GetSprite().move(ent.movement.x/16,ent.movement.y/16);

	}
}

void Gravitation(const float BaseSpeed,Entity& ent)
{
	
	if (ent.GetCollision()[1] == 0)
	{
		ent.movement.y+= BaseSpeed*1.67;
		ent.movement.y = std::min(ent.movement.y,MOVEMENTCAP*4);
	}
}

void MoveRight(const float BaseSpeed,Entity& ent, float K = 1)
{
	ent.movement.x += std::min( BaseSpeed/3 + BaseSpeed/3*2*(abs(ent.movement.x)/MOVEMENTCAP), MOVEMENTCAP/3)*K;
}

void MoveLeft(const float BaseSpeed,Entity& ent, float K = 1)
{
	ent.movement.x -= std::min(BaseSpeed/3 + BaseSpeed/3*2*(abs(ent.movement.x)/MOVEMENTCAP),MOVEMENTCAP/3)*K;
}

void RightJump(const float BaseSpeed,Entity& ent)
{
	
}

void LeftJump(const float BaseSpeed,Entity& ent)
{
	
}

void Still(const float BaseSpeed,Entity& ent, float K = 1)
{
	ent.movement.x/=1.15;
	ent.movement.y/=1.15;
}

void FollowingLogic(const float BaseSpeed,Entity& ent, Player& User)
{
	float DistanceY = User.getGlobalBounds().top+User.getGlobalBounds().height - ent.getGlobalBounds().top - ent.getGlobalBounds().height ;
	float DistanceX = User.getGlobalBounds().left+User.getGlobalBounds().width - ent.getGlobalBounds().left - ent.getGlobalBounds().width ;
	if (ent.LogicClock.getElapsedTime().asSeconds() > 0.1)
	{
 		ent.LogicClock.restart();

		if ( ( DistanceX > tileSize*2 ) && (DistanceY < tileSize*200)) 
		{
			ent.MOVE = MoveRight;
		}
		if ( ( DistanceX < -tileSize*2 ) && (DistanceY < tileSize*200)) 
		{
			ent.MOVE = MoveLeft;
		}
		if ( ( DistanceX >-tileSize*2 ) && (DistanceX < tileSize*2)) 
		{
			ent.MOVE = Still;
		}

	}
	ent.MOVE(BaseSpeed,ent,abs(pow(DistanceX,1/4))/tileSize+0.1);
}