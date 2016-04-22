#include "GameObject.h"

bool GameObject::init()
{
	if (!Node::init())
	{
		return false;
	}

	return true;
}

