#ifndef YASP_COMPONENT_SCALE_H
#define YASP_COMPONENT_SCALE_H

#include <Yasp/EntityComponent/Components/IComponent.h>
#include <Yasp/SwizzleVec.h>

namespace yasp
{
	class Scale : public IComponent, public yasp::float4
	{
	public:
		using yasp::float4::float4;
	};
}


#endif
