#ifndef YASP_ENTITY_SYSTEM_H
#define YASP_ENTITY_SYSTEM_H

#include <Yasp/EntityComponent/EntityManager.h>
#include <Yasp/EntityComponent/Entity.h>

namespace yasp
{
	class IEntitySystem
	{
	public:
		IEntitySystem(EntityManager& entityManager) : entityManager(entityManager) {}
		virtual ~IEntitySystem() = 0 {}
		
		virtual void PreFrame() = 0 {}
		virtual void Frame() = 0 {}
		virtual void PostFrame() = 0 {}


	protected:
		EntityManager& entityManager;
	};

}

#endif
