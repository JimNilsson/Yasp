#ifndef YASP_SHADER_H
#define YASP_SHADER_H

#include <Yasp/Rendering/IShader.h>
#include <Yasp/Rendering/GPUResourceID.h>
#include <Yasp/Rendering/AssignableResource.h>

namespace yasp
{
	class Shader : public GPUResourceID
	{
	public:
		Shader(uint32_t id, IShader* shader) : GPUResourceID(id), shader(shader) {}
		~Shader()
		{
			if (*(this->refCount) == 2)
				delete shader;
		}
		Shader(const Shader& other) : GPUResourceID(other), shader(other.shader) {}
		Shader& operator=(const Shader& other) 
		{
			GPUResourceID::operator=(other);
			this->shader = other.shader;
			return *this;
		}

		AssignableResource operator[](const std::string& identifier)
		{
			return (*shader)[identifier];
		}

		void Bind()
		{
			shader->Bind(*this);
		}
	private:
		IShader* shader;
	};
}

#endif
