#include "Player.h"
#include "Constants.h"

bool Player::init()
{
	if (!GameObject::init())
	{
		return false;
	}

	return true;
}

void Player::createdObject()
{
	body = PhysicsBody::createCircle(0.7f);
	body->getShape(0)->setRestitution(1.0f);
	body->getShape(0)->setFriction(1.0f);
	body->getShape(0)->setDensity(1.0f);
	body->setRotationEnable(false);
	this->setPhysicsBody(body);
}

void Player::moveRight()
{
	Vect impulse = Vect(50.0f, 0.0f);
	body->applyImpulse(impulse); 
}

void Player::jump()
{
	Vect impulse = Vect(0.0f, 100.0f);
	body->applyImpulse(impulse);
}
