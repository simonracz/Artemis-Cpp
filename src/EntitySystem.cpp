#include "EntitySystem.h"
#include "World.h"
#include "Entity.h"

using namespace std;

namespace artemis {
	
	EntitySystem::~EntitySystem() {
		world = nullptr;
	}
	
	int EntitySystem::getEntityCount(){
		return actives.getCount();
	}
	
	void EntitySystem::change(Entity& e) {
		bool contains = (systemBit & e.getSystemBits()) == systemBit;
		bool interest = (typeFlags & e.getTypeBits()) == typeFlags;
		bool excluded = (excludedTypeFlags & e.getTypeBits()).any();
		
		if((interest && !excluded) && !contains && typeFlags.any()) {
			actives.add(&e);
			e.addSystemBit(systemBit);
			added(e);
		} else if((!interest || excluded) && contains && typeFlags.any()) {
			this->remove(e);
		}
	}
	
	void EntitySystem::process() {
		if(checkProcessing()) {
			begin();
			processEntities(actives);
			end();
		}
	};
	
	void EntitySystem::setWorld(World *world) {
		this->world = world;
	};
	
	void EntitySystem::remove(Entity &e) {
		actives.remove(&e);
		e.removeSystemBit(systemBit);
		removed(e);
	};
	
	void  EntitySystem::setSystemBit(bitset<BITSIZE> bit) {
		systemBit = bit;
	}
	
};
