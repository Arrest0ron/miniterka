#pragma once

#include <SFML/Graphics.hpp>
#include "Update.h"
#include "Entity.h"

extern const int MAP_LENGTH;
extern int tileSize;
extern const float  MOVEMENTCAP;


void ViewDynamics(sf::View& UserView);

bool IsGreaterThenLimitX(sf::Vector2f& movement);

void MovementCalculation(const float BaseSpeed,Update& upd,Entity& ent);

void CollisionDynamic(const float BaseSpeed,Update& upd,Entity& ent);

void Gravitation(const float BaseSpeed,Entity& ent);

void MoveRight(const float BaseSpeed,Entity& ent, float K);

void MoveLeft(const float BaseSpeed,Entity& ent, float K);

void RightJump(const float BaseSpeed,Entity& ent);

void LeftJump(const float BaseSpeed,Entity& ent);

void Still(const float BaseSpeed,Entity& ent, float K);

void FollowingLogic(const float BaseSpeed,Entity& ent, Player& User);