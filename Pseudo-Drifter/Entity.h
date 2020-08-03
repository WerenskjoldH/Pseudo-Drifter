#ifndef ENTITY_H
#define ENTITY_H

/*
	Description: This file contains all declarations pertaining to the entity class

	An entity is literally just an ID we will use to keep things tied together
*/

class Entity
{
public:
	unsigned int id;

	Entity(const unsigned int iId);
};

#endif