#ifndef YASP_BUFFER_D3D_H
#define YASP_BUFFER_D3D_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/IBuffer.h>
#include <Yasp/Rendering/Descriptions.h>
#include <d3d11.h>


namespace yasp
{
	class BufferD3D : public IBuffer
	{
	public:
		BufferD3D(BufferDesc bufferDesc, void* initialData);
		~BufferD3D();
	private:
		ID3D11Buffer* buffer;
	};
};



#endif
