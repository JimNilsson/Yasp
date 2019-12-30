#ifndef YASP_SHADER_D3D_H
#define YASP_SHADER_D3D_H

#include <Yasp/Rendering/IShader.h>
#include <unordered_map>

namespace yasp
{
	class ShaderD3D : public yasp::IShader
	{
	public:
		ShaderD3D(uint32_t id) : IShader(id) {}
		~ShaderD3D() {}
		GPUResourceID operator[](const std::string& identifier) override final;

	private:
		std::unordered_map<std::string, GPUResourceID> shaderResources;
	};
}


#endif
