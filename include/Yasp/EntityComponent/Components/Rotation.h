#ifndef YASP_COMPONENT_ROTATION_H
#define YASP_COMPONENT_ROTATION_H

#include <Yasp/EntityComponent/Components/IComponent.h>
#include <Yasp/SwizzleVec.h>

namespace yasp
{
	class Rotation : public IComponent, public yasp::quaternion
	{
	public:
		using yasp::quaternion::quaternion;
		
	};
}


#endif
