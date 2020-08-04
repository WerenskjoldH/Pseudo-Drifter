#include "DrawableComponent.h"

DrawableComponent::DrawableComponent(rn::vector4f iColor, int radius) : color{ iColor }, radius{ radius }, Component("DrawableComponent")
{
}
