#ifndef YASP_ISHADER_H
#define YASP_ISHADER_H

#include <Yasp/Rendering/GPUResourceID.h>
#include <string>
namespace yasp
{
	class IShader : public GPUResourceID
	{
	public:
		IShader(uint32_t id) : GPUResourceID(id) {};
		virtual ~IShader() = 0 {};
		virtual GPUResourceID operator[](const std::string&) = 0;
	};
}


#endif
