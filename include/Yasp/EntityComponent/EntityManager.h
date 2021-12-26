#ifndef YASP_ENTITY_MANAGER_H
#define YASP_ENTITY_MANAGER_H

#include <Yasp/Config.h>
#include <Yasp/EntityComponent/Entity.h>
#include <Yasp/EntityComponent/ComponentContainer.h>

#include <Yasp/detail.h>
#include <Yasp/Rendering/AssignableMemory.h>
#include <Yasp/Rendering/AssignableResource.h>
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


		void Provide(const std::string& thing, std::function<bool(Entity, AssignableMemory)> callback)
		{
			dataProviders[thing] = callback;
		};

		void Provide(const std::string& resource, std::function<bool(Entity, AssignableResource)> callback)
		{
			resourceProviders[resource] = callback;
		}

		void Request(const std::string& thing, Entity e, AssignableMemory buffer)
		{
			if (auto it = dataProviders.find(thing); it != dataProviders.end())
			{
				auto& provider = it->second;
				provider(e, buffer);
			}
		}

		void Request(const std::string& thing, Entity e, AssignableResource resource)
		{
			if (auto it = resourceProviders.find(thing); it != resourceProviders.end())
			{
				auto& provider = it->second;
				provider(e, resource);
			}
		}

		void PreFrame()
		{
			
		}

		void Frame()
		{
			
		}
		
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

		template <typename F>
		void OnUpdate(const F& fn)
		{
			_OnUpdate(typename detail::deduce_type<decltype(&F::operator())>::type(fn));
		}

		
		template <typename T>
		const T& Read(Entity entity);

		template <typename T>
		void Update(Entity entity, const T& component);

		template <typename First, typename Second, typename ...Rest>
		void Update(Entity entity, const First& first, const Second& second, const Rest&... rest);

		void FireUpdateCallbacks()
		{
			for (auto it : onComponentUpdateCallbacks)
			{
				auto mask = it.first;
				auto callbacks = it.second;
				for (auto callback : callbacks)
				{
					for (auto it2 : updatedEntities)
					{
						auto entity = it2.first;
						auto updatedMask = it2.second;
						if (updatedMask & mask)
						{
							callback(entity);
						}
					}
				}
			}
			updatedEntities.clear();
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
		void _OnUpdate(const std::function<void(Entity, First&)>& func);

		template <typename First, typename Second, typename ...Rest>
		void _OnUpdate(const std::function<void(Entity, First&, Second&, Rest&...)>& func);

		template <typename First>
		void _ForEach(const std::function<void(Entity, First&)>& func);

		template <typename First, typename Second, typename ...Rest>
		void _ForEach(const std::function<void(Entity, First&, Second&, Rest&...)>& func);

		template<typename T>
		T& _Retrieve(Entity entity);

		std::vector<uint8_t> entityGenerations;
		std::vector<uint32_t> freeIndices;
		std::unordered_map<std::string, std::function<void(Entity, AssignableMemory)>> dataProviders;
		std::unordered_map<std::string, std::function<void(Entity, AssignableResource)>> resourceProviders;
		std::unordered_map<uint64_t, ComponentContainer> componentPools;
		std::unordered_map<Entity, uint64_t, Entity::EntityHasher> componentMasks;
		std::unordered_map<Entity, uint64_t, Entity::EntityHasher> updatedEntities;
		std::unordered_map<uint64_t, std::vector<std::function<void(Entity)>>> onComponentUpdateCallbacks;
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
			if ((mask & it.second) && IsAlive(it.first))
			{
				auto args = std::tuple<Entity, First&>(it.first, container[it.first]);
				std::apply(func, args);
				updatedEntities[it.first] |= mask;
			}
		}
		FireUpdateCallbacks();
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
				updatedEntities[it.first] |= mask;
			}
		}
		FireUpdateCallbacks();
	}

	template<typename First>
	inline void EntityManager::_OnUpdate(const std::function<void(Entity, First&)>& func)
	{
		auto mask = ConstructMask<First>();
		auto result = componentPools.try_emplace(TypeId<First>(), sizeof(First));
		auto& container = (*result.first).second;

		onComponentUpdateCallbacks[mask].push_back([&, func, mask](Entity e) -> void {
			auto args = std::tuple<Entity, First&>(e, container[e]);
			std::apply(func, args);
		});
	}

	template <typename First, typename Second, typename ...Rest>
	inline void EntityManager::_OnUpdate(const std::function<void(Entity, First&, Second&, Rest&...)>& func)
	{
		auto mask = ConstructMask<First, Second, Rest...>();
		onComponentUpdateCallbacks[mask].push_back([&, func, mask](Entity e) -> void {
			auto entityMask = componentMasks[e];
			if ((mask & entityMask) == mask)
			{
				auto comps = SafeRetrieve<First, Second, Rest...>(e);
				auto args = std::tuple_cat(std::tuple<Entity>(e), comps);
				std::apply(func, args);
			}
		});
	}

	template<typename T>
	inline const T & EntityManager::Read(Entity entity)
	{
		auto result = componentPools.try_emplace(TypeId<T>(), sizeof(T));
		auto& container = (*result.first).second;
		return container[entity];
	}

	template<typename T>
	inline void EntityManager::Update(Entity entity, const T& component)
	{
		auto mask = ConstructMask<T>();
		auto result = componentPools.try_emplace(TypeId<T>(), sizeof(T));
		auto& container = (*result.first).second;
		container[entity] = component;
		updatedEntities[entity] |= mask;
	}

	template<typename First, typename Second, typename ...Rest>
	inline void EntityManager::Update(Entity entity, const First & first, const Second & second, const Rest & ...rest)
	{
		Update(entity, first);
		Update(entity, second, rest...);
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
