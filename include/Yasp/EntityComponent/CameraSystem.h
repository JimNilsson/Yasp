#ifndef YASP_CAMERA_SYSTEM_H
#define YASP_CAMERA_SYSTEM_H

#include <Yasp/EntityComponent/IEntitySystem.h>
#include <Yasp/EntityComponent/EntityManager.h>
#include <Yasp/EntityComponent/Components/Position.h>
#include <Yasp/EntityComponent/Components/Rotation.h>
#include <Yasp/EntityComponent/Components/Scale.h>
#include <Yasp/EntityComponent/Components/Lens.h>
#include <Yasp/SwizzleVec.h>
#include <unordered_map>
#include <vector>
#include <functional>

namespace yasp
{

	class CameraSystem : public IEntitySystem
	{
	public:
		CameraSystem(EntityManager& entityManager) : IEntitySystem(entityManager)
		{
			entityManager.Provide("WorldViewProjectionMatrix", [&](Entity e, AssignableMemory bufferSegment) -> bool {
				if (activeIndex >= viewMatrices.size())
				{
					return false;
				}
				const auto& pos = entityManager.Read<Position>(e);
				const auto& rot = entityManager.Read<Rotation>(e);
				const auto& scale = entityManager.Read<Scale>(e);
				auto world = mat4::Scale(scale.xyz) * mat4::RotationQuaternion(rot) * mat4::Translation(pos.xyz);
				const auto& viewProjection = viewProjectionMatrices[activeIndex];
				const auto worldViewProjection = world * viewProjection;
				bufferSegment = ~worldViewProjection;
				return true;
			});

			entityManager.OnUpdate([&](Entity e, Position& pos, Rotation& rot, Lens& lens) -> void {
				if (auto it = entityIndices.find(e); it != entityIndices.end())
				{
					auto index = it->second;
					const auto forward = float4(0.0f, 0.0f, 1.0f, 0.0f);
					const auto up = float4(0.0f, 1.0f, 0.0f, 0.0f);
					auto rotDir = mat4::RotationQuaternion(rot);
					auto dir = rotDir * forward;
					auto upDir = rotDir * up;
					const auto viewMatrix = mat4::LookToLH(pos.xyz, dir.xyz, upDir.xyz);
					const auto projectionMatrix = lens.PerspectiveLH();
					const auto viewProjectionMatrix = viewMatrix * projectionMatrix;

					viewMatrices[index] = viewMatrix;
					projectionMatrices[index] = projectionMatrix;
					viewProjectionMatrices[index] = viewProjectionMatrix;
				}
			});
		}
		~CameraSystem() {}

		void PreFrame() override final {}
		void Frame() override final {}
		void PostFrame() override final {}

		bool SetActiveCamera(Entity e)
		{
			if (auto it = entityIndices.find(e); it != entityIndices.end())
			{
				auto index = it->second;
				activeIndex = index;
				return true;
			}
			const auto& pos = entityManager.Read<Position>(e);
			const auto& rot = entityManager.Read<Rotation>(e);
			const auto& lens = entityManager.Read<Lens>(e);
			
			const auto forward = float4(0.0f, 0.0f, 1.0f, 0.0f);
			const auto up = float4(0.0f, 1.0f, 0.0f, 0.0f);
			auto rotDir = mat4::RotationQuaternion(rot);
			auto dir = rotDir * forward;
			auto upDir = rotDir * up;
			const auto viewMatrix = mat4::LookToLH(pos.xyz, dir.xyz, upDir.xyz);
			const auto projectionMatrix = lens.PerspectiveLH();
			viewMatrices.push_back(viewMatrix);
			projectionMatrices.push_back(projectionMatrix);
			viewProjectionMatrices.push_back(viewMatrix * projectionMatrix);
			size_t index = viewMatrices.size() - 1;
			entityIndices[e] = index;
			activeIndex = index;
			return true;
		}

		
	private:
		std::unordered_map<Entity, size_t, Entity::EntityHasher> entityIndices;
		size_t activeIndex = 0;
		std::vector<mat4> viewMatrices;
		std::vector<mat4> projectionMatrices;
		std::vector<mat4> viewProjectionMatrices;

	};

};



#endif
