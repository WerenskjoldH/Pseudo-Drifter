#ifndef COMPONENT_H
#define COMPONENT_H
	
/*
	Description: This file contains the component class declaration

	This class really just has to store a string containing the name of the component

	To-do:
		* Should componentName even be used?
*/

#include <string>
#include <string_view>

class Component
{
protected:
	std::string componentName;

public: 
	Component(const std::string iComponentName);
	virtual ~Component() {};

	const std::string& GetComponentName() const;

};

#endif