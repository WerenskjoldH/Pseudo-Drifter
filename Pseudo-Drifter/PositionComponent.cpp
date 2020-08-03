#include "PositionComponent.h"

PositionComponent::PositionComponent(rn::vector3f worldPosition) : dV{worldPosition}, Component("PositionComponent")
{
}
