#ifndef YASP_ENTITY_MANAGER_H
#define YASP_ENTITY_MANAGER_H

#include <Yasp/Config.h>
#include <Yasp/EntityComponent/Entity.h>
#include <Yasp/EntityComponent/ComponentContainer.h>
#include <Yasp/detail.h>
#include <unordered_map>
#include <any>
#include <atomic>
#include <functional>
#include <optional>
#include <tuple>
#include <utility>

extern std::atomic_uint64_t TypeIdCounter;
template <typename T>
uint64_t TypeId()
{
	static uint64_t id = TypeIdCounter ? TypeIdCounter = TypeIdCounter << 1 : TypeIdCounter = 1;
	return id;
}



namespace yasp
{
	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();
		NOMOVORCOPY(EntityManager);

		Entity Create();
		void Destroy(Entity entity);
		bool IsAlive(Entity entity);

		template <typename First>
		void Register(Entity entity);

		template <typename First>
		void Register(Entity entity, const First& first);

		template <typename First, typename Second, typename ...Rest>
		void Register(Entity entity);

		template <typename First, typename Second, typename ...Rest>
		void Register(Entity entity, const First& first, const Second& second, const Rest&... rest);

		template <typename F>
		auto ForEach(const F& fn)
		{
			_ForEach(typename detail::deduce_type< decltype(&F::operator()) >::type(fn));
		}

	private:
		template <typename T>
		void RegisterComponent(Entity entity);

		template <typename T>
		void RegisterComponent(Entity entity, const T& component);

		template <typename First>
		uint64_t ConstructMask();

		template <typename First, typename Second, typename ...Rest>
		uint64_t ConstructMask();

		template <typename First>
		std::tuple<First&> SafeRetrieve(Entity entity);

		template <typename First, typename Second, typename ...Rest>
		std::tuple<First&, Second&, Rest&...> SafeRetrieve(Entity entity);		

		template <typename First>
		void _ForEach(const std::function<void(Entity, First&)>& func);

		template <typename First, typename Second, typename ...Rest>
		void _ForEach(const std::function<void(Entity, First&, Second&, Rest&...)>& func);

		template<typename T>
		T& _Retrieve(Entity entity);

		std::vector<uint8_t> entityGenerations;
		std::vector<uint32_t> freeIndices;
		std::unordered_map<uint64_t, ComponentContainer> componentPools;
		std::unordered_map<uint64_t, std::unordered_map<Entity, std::any, Entity::EntityHasher>> componentPools2;
		std::unordered_map<Entity, uint64_t, Entity::EntityHasher> componentMasks;
	};

	template<typename First>
	inline void EntityManager::Register(Entity entity)
	{
		RegisterComponent<First>(entity);
	}

	template<typename First, typename Second, typename ...Rest>
	inline void EntityManager::Register(Entity entity)
	{
		RegisterComponent<First>(entity);
		Register<Second, Rest...>();
	}

	template<typename First>
	inline void EntityManager::Register(Entity entity, const First & first)
	{
		RegisterComponent(entity, first);
	}

	template<typename First, typename Second, typename ...Rest>
	inline void EntityManager::Register(Entity entity, const First & first, const Second & second, const Rest & ...rest)
	{
		RegisterComponent(entity, first);
		Register(entity, second, rest...);
	}

	template<typename First>
	inline void EntityManager::_ForEach(const std::function<void(Entity, First&)>& func)
	{
		auto mask = ConstructMask<First>();
		auto result = componentPools.try_emplace(TypeId<First>(), sizeof(First));
		auto& container = (*result.first).second;
		for (auto& it : componentMasks)
		{
			if (mask == it.second && IsAlive(it.first))
			{
				auto args = std::tuple<Entity, First&>(it.first, container[it.first]);
				std::apply(func, args);
			}
		}
	}

	template<typename First, typename Second, typename ...Rest>
	inline void EntityManager::_ForEach(const std::function<void(Entity, First&, Second&, Rest&...)>& func)
	{
		auto mask = ConstructMask<First, Second, Rest...>();
		for (auto& it : componentMasks)
		{
			auto f = mask & it.second;
			if ((mask & it.second) == mask && IsAlive(it.first))
			{
				auto comps = SafeRetrieve<First, Second, Rest...>(it.first);
				auto args = std::tuple_cat(std::tuple<Entity>(it.first), comps);
				std::apply(func, args);
			}
		}
	}

	template<typename T>
	inline void EntityManager::RegisterComponent(Entity entity)
	{	
		componentMasks[entity] |= ConstructMask<T>();
		auto result = componentPools.try_emplace(TypeId<T>(), sizeof(T));
		auto& container = (*result.first).second;
		container[entity] = T();
	}

	template<typename T>
	inline void EntityManager::RegisterComponent(Entity entity, const T& component)
	{
		componentMasks[entity] |= ConstructMask<T>();
		auto result = componentPools.try_emplace(TypeId<T>(), sizeof(T));
		auto& container = (*result.first).second;
		container[entity] = component;
	}

	template<typename First>
	inline uint64_t EntityManager::ConstructMask()
	{
		return TypeId<First>();
	}

	template<typename First, typename Second, typename ...Rest>
	inline uint64_t EntityManager::ConstructMask()
	{
		return TypeId<First>() | ConstructMask<Second, Rest...>();
	}

	template<typename First>
	inline std::tuple<First&> EntityManager::SafeRetrieve(Entity entity)
	{
		return std::tuple<First&>(_Retrieve<First>(entity));
	}

	template<typename First, typename Second, typename ...Rest>
	inline std::tuple<First&, Second&, Rest&...> EntityManager::SafeRetrieve(Entity entity)
	{
		return std::tuple_cat(SafeRetrieve<First>(entity), SafeRetrieve<Second, Rest...>(entity));
	}

	template<typename T>
	inline T& EntityManager::_Retrieve(Entity entity)
	{
		auto result = componentPools.try_emplace(TypeId<T>(), sizeof(T));
		auto& container = (*result.first).second;
		return container[entity];
	}
}

#endif
