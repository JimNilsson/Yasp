#ifndef YASP_SHADER_D3D_H
#define YASP_SHADER_D3D_H

#include <Yasp/Rendering/IShader.h>
#include <Yasp/Rendering/Descriptions.h>
#include <string>
#include <unordered_map>

namespace yasp
{
	class GPUResourceManagerD3D;
	class ShaderD3D : public yasp::IShader
	{
	public:
		ShaderD3D(ShaderType type, GPUResourceManagerD3D* resourceManager) : IShader(type), resourceManager(resourceManager) {}
		~ShaderD3D() {}
		AssignableResource operator[](const std::string& identifier) override final;
		void SetResource(const std::string& identifier, const GPUResourceID& id) override final;
		void Bind(const GPUResourceID&) override final;
		void RegisterBinding(const std::string& identifier, ShaderResourceType type, int32_t slot);
	private:
		
		struct ShaderResourceBinding
		{
			ShaderResourceType type;
			int32_t slot;
			GPUResourceID resource;
		};
		std::unordered_map<std::string, ShaderResourceBinding> shaderResources;
		GPUResourceManagerD3D* resourceManager;
	};
}


#endif
