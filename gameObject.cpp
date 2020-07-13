#include "gameObject.h"

GameObject::~GameObject()
{
}

rn::dualVector& GameObject::getDualVector()
{
	return positionDV;
}
