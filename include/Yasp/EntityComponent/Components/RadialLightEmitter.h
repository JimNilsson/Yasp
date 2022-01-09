#ifndef YASP_RADIAL_LIGHT_EMITTER_H
#define YASP_RADIAL_LIGHT_EMITTER_H

#include <Yasp/EntityComponent/Components/IComponent.h>
#include <Yasp/SwizzleVec.h>

namespace yasp
{
	class RadialLightEmitter
	{
	public:
		RadialLightEmitter(float3 color, float falloffLinear, float3 direction, float directionBias, float3 position, float falloffExponential)
			: color(color), falloffLinear(falloffLinear), direction(direction), directionBias(directionBias), position(position), falloffExponential(falloffExponential) {}
		RadialLightEmitter()
			: color({ 1.0f, 1.0f, 1.0f }), falloffLinear(0.0f), direction({ 1.0f, 0.0f, 0.0f }), directionBias(0.0f), position({ 0.0f, 0.0f, 0.0f }), falloffExponential(1.0f) {}
		float3 color;
		float falloffLinear;
		float3 direction;
		float directionBias;
		float3 position;
		float falloffExponential;
	};
}


#endif
