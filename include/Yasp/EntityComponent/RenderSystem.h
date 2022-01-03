#ifndef YASP_RENDER_SYSTEM_H
#define YASP_RENDER_SYSTEM_H

#include <Yasp/EntityComponent/IEntitySystem.h>
#include <Yasp/EntityComponent/EntityManager.h>
#include <Yasp/Rendering/RenderContext.h>
#include <Yasp/Rendering/GPUBuffer.h>
#include <Yasp/Rendering/GPUResourceID.h>
#include <Yasp/Rendering/Descriptions.h>

namespace yasp
{
	namespace gpu_components
	{
		class VertexBuffer : public GPUResourceID { using GPUResourceID::GPUResourceID; };
		class IndexBuffer : public GPUResourceID { using GPUResourceID::GPUResourceID; };
		class VertexShader : public GPUResourceID {
		public:
			using GPUResourceID::GPUResourceID;
			VertexShader() : GPUResourceID() {}
			VertexShader(const  GPUResourceID& o) : GPUResourceID(o) {}
		};
		class PixelShader : public GPUResourceID { using GPUResourceID::GPUResourceID; };
		class Rasterizer : public GPUResourceID { using GPUResourceID::GPUResourceID; };
		using Toplogy = yasp::Topology;
		class Viewport : public yasp::Viewport { using yasp::Viewport::Viewport; };
		
	}
	
	class RenderSystem : public IEntitySystem
	{
	public:
		RenderSystem(EntityManager& entityManager, RenderContext& renderContext) : IEntitySystem(entityManager), renderContext(renderContext) {}
		~RenderSystem() {};


	private:
		RenderContext& renderContext;

	

	};
}

#endif
