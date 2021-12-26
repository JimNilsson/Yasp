#include <Yasp/Rendering/ShaderD3D.h>
#include <Yasp/Rendering/AssignableResource.h>
#include <Yasp/Rendering/GPUResourceManagerD3D.h>
#include <Yasp/Rendering/Descriptions.h>
#include <Yasp/Rendering/Enums.h>
#include <Yasp/Rendering/GPUBuffer.h>

yasp::AssignableResource yasp::ShaderD3D::operator[](const std::string & identifier)
{
	return AssignableResource(this, identifier);
}

void yasp::ShaderD3D::SetResource(const std::string & identifier, const GPUResourceID & id)
{
	if (auto f = shaderResources.find(identifier); f != shaderResources.end())
	{
		f->second.resource = id;
	}
}

void yasp::ShaderD3D::Bind(const GPUResourceID& id)
{
	for (auto& it : shaderResources)
	{
		switch (it.second.type)
		{
		case ShaderResourceType::TEXTURE:
			resourceManager->SetShaderTextureViews(this->type, &it.second.resource, it.second.slot, 1);
			break;
		case ShaderResourceType::SAMPLER:
			resourceManager->SetShaderSamplers(this->type, &it.second.resource, it.second.slot, 1);
			break;
		case ShaderResourceType::BUFFER:
			resourceManager->SetShaderBuffers(this->type, &it.second.resource, it.second.slot, 1);
			break;
		}
	}


	switch (this->type)
	{
	case ShaderType::VERTEX:
		resourceManager->SetVertexShader(id);
		break;
	case ShaderType::GEOMETRY:
		resourceManager->SetGeometryShader(id);
		break;
	case ShaderType::PIXEL:
		resourceManager->SetPixelShader(id);
		break;
	case ShaderType::COMPUTE:
	//	resourceManager->SetComputeShader(id);
		break;
	}

}

yasp::GPUBuffer yasp::ShaderD3D::GetBuffer(const std::string & identifier)
{
	if (auto f = shaderBuffers.find(identifier); f != shaderBuffers.end())
	{
		return f->second;
	}
	return GPUBuffer();
}

void yasp::ShaderD3D::OnEachBuffer(std::function<void(GPUBuffer)> callback)
{
	for (auto it : shaderBuffers)
	{
		callback(it.second);
	}
}

void yasp::ShaderD3D::RegisterBinding(const std::string& identifier, ShaderResourceType type, int32_t slot)
{
	shaderResources[identifier] = { type, slot, GPUResourceID()};
}

void yasp::ShaderD3D::RegisterBuffer(const std::string & identifier, GPUBuffer buffer, int32_t slot)
{
	shaderBuffers[identifier] = buffer;
	shaderResources[identifier] = { ShaderResourceType::BUFFER, slot, buffer };
}
