#include <Yasp/EntityComponent/ComponentContainer.h>

yasp::ComponentContainer::ComponentContainer(size_t componentSize) : componentSize(componentSize), count(0), capacity(256)
{
	components = new uint8_t[capacity * componentSize];
}

yasp::ComponentContainer::~ComponentContainer()
{
	delete[] components;
}

yasp::AssignableMemory yasp::ComponentContainer::operator[](Entity entity)
{
	if (auto f = entityToIndex.find(entity); f != entityToIndex.end())
	{
		const auto ptr = static_cast<uint8_t*>(components) + (componentSize * f->second);
		return AssignableMemory(ptr, componentSize);
	}
	if (count == capacity)
	{
		capacity += 256;
		auto newData = new uint8_t[capacity * componentSize];
		memcpy(newData, components, count * componentSize);
		delete[] components;
		components = newData;
	}
	auto index = (entityToIndex[entity] = count++);
	const auto ptr = static_cast<uint8_t*>(components) + (componentSize * index);
	return AssignableMemory(ptr, componentSize);
}
