#include <Yasp/EntityComponent/TransformSystem.h>


void yasp::TransformSystem::PreFrame()
{

}

void yasp::TransformSystem::Frame()
{
}

void yasp::TransformSystem::PostFrame()
{
}

void yasp::TransformSystem::Register(const Entity & entity, const Position & position, const Rotation & rotation, const Scale & scale)
{
	auto scaleMat = mat4::Scale(scale.xyz);
	auto rotMat = mat4::RotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	auto posMat = mat4::Translation(position.xyz);
	auto transform = scaleMat * rotMat * posMat;
	worldMatrices.push_back(transform);
	rotationMatrices.push_back(rotMat);
	translationMatrices.push_back(posMat);
	scalingMatrices.push_back(scaleMat);
	auto index = worldMatrices.size() - 1;
	entityIndices[entity] = index;
	entityManager.Register(entity, position, rotation, scale);
}

const yasp::mat4& yasp::TransformSystem::GetTransform(const Entity & entity)
{
	if (auto it = entityIndices.find(entity); it != entityIndices.end())
	{
		auto index = it->second;
		return worldMatrices[index];
	}
	return mat4::Identity();
}
