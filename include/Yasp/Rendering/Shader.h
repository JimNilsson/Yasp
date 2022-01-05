#ifndef YASP_SHADER_H
#define YASP_SHADER_H

#include <Yasp/Rendering/IShader.h>
#include <Yasp/Rendering/GPUResourceID.h>
#include <Yasp/Rendering/GPUBuffer.h>
#include <Yasp/Rendering/AssignableResource.h>
#include <functional>

namespace yasp
{
	class Shader : public GPUResourceID
	{
	public:
		Shader() : GPUResourceID(), shader(nullptr) {}
		Shader(uint32_t id, IShader* shader) : GPUResourceID(id), shader(shader) {}
		~Shader()
		{
			if (this->refCount != nullptr && *(this->refCount) == 2)
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

		GPUBuffer GetBuffer(const std::string& identifier)
		{
			return shader->GetBuffer(identifier);
		}

		void OnEachBuffer(std::function<void(GPUBuffer)> callback)
		{
			shader->OnEachBuffer(callback);
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
