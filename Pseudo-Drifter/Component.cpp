#include "Component.h"

Component::Component(const std::string iComponentName) : componentName {iComponentName}
{
}

const std::string& Component::GetComponentName() const
{
	return componentName;
}
