#ifndef YASP_ASSIGNABLE_RESOURCE
#define YASP_ASSIGNABLE_RESOURCE

#include <Yasp/Rendering/IShader.h>
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
	private:
		IShader* shader;
		std::string identifier;
	};
}

#endif
