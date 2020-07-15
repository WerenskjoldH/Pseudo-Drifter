#include "gameObject.h"

GameObject::~GameObject()
{
}

rn::dualVector& GameObject::GetDualVector()
{
	return dV;
}
