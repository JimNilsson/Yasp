#ifndef YASP_LENS_H
#define YASP_LENS_H

#include <Yasp/EntityComponent/Components/IComponent.h>
#include <Yasp/SwizzleVec.h>

namespace yasp
{
	class Lens : public IComponent
	{
	public:
		Lens() : fieldOfView(yasp::PI<float> / 4.0f), aspectRatio(16.0f / 9.0f), nearPlane(1.0f), farPlane(100.0f) {}
		Lens(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) : fieldOfView(fieldOfView), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane) {}
		~Lens() {}
		float fieldOfView;
		float aspectRatio;
		float nearPlane;
		float farPlane;

		mat4 PerspectiveLH() const
		{
			return mat4::PerspectiveLH(fieldOfView, aspectRatio, nearPlane, farPlane);
		}
	};
}

#endif
