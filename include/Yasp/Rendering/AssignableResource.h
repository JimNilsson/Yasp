#ifndef YASP_ASSIGNABLE_RESOURCE
#define YASP_ASSIGNABLE_RESOURCE

#include <Yasp/Rendering/IShader.h>
#include <Yasp/Rendering/GPUBuffer.h>
#include <Yasp/Rendering/AssignableMemory.h>
#include <Yasp/Rendering/GPUBufferVariable.h>
#include <string>

namespace yasp
{
	class AssignableResource
	{
	public:
		AssignableResource(IShader* shader, const std::string& identifier) : shader(shader), identifier(identifier) {};
		~AssignableResource() {};
		void operator=(const GPUResourceID& other)
		{
			shader->SetResource(identifier, other);
		}
		GPUBufferVariable operator[](const std::string& var)
		{
			return shader->GetBuffer(identifier)[var];
		}
		void Update()
		{
			shader->GetBuffer(identifier).Update();
		}


	private:
		IShader* shader;
		std::string identifier;
	};
}

#endif
