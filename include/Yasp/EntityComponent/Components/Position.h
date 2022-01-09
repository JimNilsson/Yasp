#ifndef YASP_COMPONENT_POSITION_H
#define YASP_COMPONENT_POSITION_H

#include <Yasp/EntityComponent/Components/IComponent.h>
#include <Yasp/SwizzleVec.h>

namespace yasp
{
	class Position : public yasp::float4
	{
	public:
		using yasp::float4::float4;
	};
}


#endif
