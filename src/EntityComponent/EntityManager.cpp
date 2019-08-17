#include <Yasp/EntityComponent/EntityManager.h>

std::atomic_uint64_t TypeIdCounter(0);

yasp::EntityManager::EntityManager()
{

}

yasp::EntityManager::~EntityManager()
{

}

yasp::Entity yasp::EntityManager::Create()
{
	if (freeIndices.size() == 0)
	{
		const auto index = entityGenerations.size();
		entityGenerations.push_back(0);
		return Entity(0, index);
	}
	const auto index = freeIndices.back();
	freeIndices.pop_back();
	return Entity(entityGenerations[index], index);
}

void yasp::EntityManager::Destroy(Entity entity)
{
	const auto index = entity.Index();
	if(index < entityGenerations.size() && entity.Gen() == entityGenerations[index])
	{ 
		entityGenerations[index]++;
		freeIndices.push_back(index);
	}
}

bool yasp::EntityManager::IsAlive(Entity entity)
{
	return entityGenerations[entity.Index()] == entity.Gen();
}
