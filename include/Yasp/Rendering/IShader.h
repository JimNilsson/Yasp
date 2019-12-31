#ifndef YASP_ISHADER_H
#define YASP_ISHADER_H

#include <Yasp/Rendering/GPUResourceID.h>
#include <Yasp/Rendering/Enums.h>
#include <string>
namespace yasp
{
	class AssignableResource;
	class IShader
	{
	public:
		IShader(ShaderType type) : type(type) {};
		virtual ~IShader() = 0 {};
		virtual AssignableResource operator[](const std::string&) = 0;
		virtual void SetResource(const std::string& identifier, const GPUResourceID& id) = 0;
		virtual void Bind(const GPUResourceID&) = 0;
	protected:
		ShaderType type;
	};
}


#endif
