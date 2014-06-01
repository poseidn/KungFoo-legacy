#pragma once

#include <boost/ptr_container/ptr_list.hpp>
#include <map>
#include <list>

class Entity;

// needs to be a list to be able to insert items in the middle
//typedef boost::ptr_list<Entity> EntityList;
typedef std::list<Entity*> EntityList;

// this list type can be used by classes using the EntityEngine to keep their
// own private list of entities and destroy them with one call
typedef std::list<Entity*> ManagedEntityList;

typedef std::list<Entity*> ManagedStaticEntityList;


// todo: also try using unordered_map
typedef std::map<std::string, Entity *> EntityIndex;
