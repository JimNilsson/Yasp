#include <Yasp/Rendering/GPUResourceManagerD3D.h>
#include <assert.h>

yasp::GPUResourceManagerD3D::GPUResourceManagerD3D(RenderContextD3D * renderContext) : resourceCounter(0)
{
	this->device = renderContext->Device();
	this->deviceContext = renderContext->DeviceContext();
}

yasp::GPUResourceManagerD3D::~GPUResourceManagerD3D()
{
	for (auto& r : resourceMap)
	{
		r.second.resource->Release();
	}
}

yasp::GPUResourceID yasp::GPUResourceManagerD3D::CreateBuffer(BufferDesc bufferDesc, void * initialData)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = bufferDesc.size;
	bd.StructureByteStride = bufferDesc.byteStride;
	switch (bufferDesc.usage)
	{
	case Usage::GPU_READ_WRITE:
		bd.Usage = D3D11_USAGE_DEFAULT;
		break;
	case Usage::GPU_READ:
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		break;
	case Usage::CPU_READ_WRITE:
		bd.Usage = D3D11_USAGE_STAGING;
		break;
	case Usage::GPU_READ_CPU_WRITE:
		bd.Usage = D3D11_USAGE_DYNAMIC;
		break;
	}

	bd.BindFlags |= bufferDesc.bind & BufferBinding::BIND_VERTEX_BUFFER ? D3D11_BIND_VERTEX_BUFFER : 0U;
	bd.BindFlags |= bufferDesc.bind & BufferBinding::BIND_INDEX_BUFFER ? D3D11_BIND_INDEX_BUFFER : 0U;
	bd.BindFlags |= bufferDesc.bind & BufferBinding::BIND_SHADER_BUFFER ? D3D11_BIND_INDEX_BUFFER : 0U;
	bd.BindFlags |= bufferDesc.bind & BufferBinding::BIND_STREAM_OUT ? D3D11_BIND_INDEX_BUFFER : 0U;
	bd.BindFlags |= bufferDesc.bind & BufferBinding::BIND_COMPUTE_ACCESS ? D3D11_BIND_INDEX_BUFFER : 0U;

	bd.CPUAccessFlags |= bufferDesc.usage == Usage::CPU_READ_WRITE ? D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE : 0U;
	bd.CPUAccessFlags |= bufferDesc.usage == Usage::GPU_READ_CPU_WRITE ? D3D11_CPU_ACCESS_WRITE : 0U;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = initialData;

	ID3D11Buffer* buffer;

	HRESULT hr = device->CreateBuffer(&bd, &sd, &buffer);
	assert(SUCCEEDED(hr));

	GPUResourceID id = GPUResourceID(resourceCounter++);
	resourceMap[id] = { buffer };
	return id;
}
