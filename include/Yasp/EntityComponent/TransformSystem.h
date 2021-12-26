#ifndef YASP_TRANSFORM_SYSTEM_H
#define YASP_TRANSFORM_SYSTEM_H
#include <Yasp/EntityComponent/IEntitySystem.h>
#include <Yasp/SwizzleVec.h>
#include <Yasp/EntityComponent/Components/Position.h>
#include <Yasp/EntityComponent/Components/Rotation.h>
#include <Yasp/EntityComponent/Components/Scale.h>


#include <unordered_map>
#include <vector>

namespace yasp
{
	class TransformSystem : public IEntitySystem
	{
	public:
		TransformSystem(EntityManager& entityManager) : IEntitySystem(entityManager)
		{
			entityManager.Provide("WorldMatrix", [&](Entity e, AssignableMemory buffer) -> bool {
				if (auto it = entityIndices.find(e); it != entityIndices.end())
				{
					buffer = ~worldMatrices[it->second];
					return true;
				}
				return false;
			});

			entityManager.Provide("RotationMatrix", [&](Entity e, AssignableMemory buffer) -> bool {
				if (auto it = entityIndices.find(e); it != entityIndices.end())
				{
					buffer = ~rotationMatrices[it->second];
					return true;
				}
				return false;
			});

			entityManager.OnUpdate([&](Entity e, Position& p, Rotation& r, Scale& s) {
				if (auto it = entityIndices.find(e); it != entityIndices.end())
				{
					const auto index = it->second;
					const auto scaleMatrix = mat4::Scale(float3(s.xyz));
					const auto rotMatrix = mat4::RotationQuaternion(r);
					const auto translationMatrix = mat4::Translation(float3(p.xyz));
					worldMatrices[index] = scaleMatrix * rotMatrix * translationMatrix;
					scalingMatrices[index] = scaleMatrix;
					rotationMatrices[index] = rotMatrix;
					translationMatrices[index] = translationMatrix;
				}
			});
		}
		virtual ~TransformSystem() {}

		void PreFrame() override final;
		void Frame() override final;
		void PostFrame() override final;
		void Register(const Entity& entity, const Position& position, const Rotation& rotation, const Scale& scale);

		const mat4& GetTransform(const Entity& entity);

	private:
		std::unordered_map<Entity, size_t, Entity::EntityHasher> entityIndices;
		std::vector<mat4> worldMatrices;
		std::vector<mat4> scalingMatrices;
		std::vector<mat4> rotationMatrices;
		std::vector<mat4> translationMatrices;
	};
}


#endif

