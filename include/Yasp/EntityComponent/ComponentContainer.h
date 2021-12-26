#ifndef YASP_COMPONENT_CONTAINER_H
#define YASP_COMPONENT_CONTAINER_H

#include <Yasp/EntityComponent/Entity.h>
#include <Yasp/Rendering/AssignableMemory.h>
#include <functional>
#include <unordered_map>

namespace yasp
{
	class ComponentContainer
	{
	public:
		ComponentContainer(size_t componentSize);
		~ComponentContainer();
		
		AssignableMemory operator[](Entity entity);

	private:
		std::unordered_map<Entity, size_t, Entity::EntityHasher> entityToIndex;
		void* components;
		size_t count;
		size_t capacity;
		size_t componentSize;
	};
};

#endif
