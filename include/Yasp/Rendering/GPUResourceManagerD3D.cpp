#include <Yasp/Rendering/GPUResourceManagerD3D.h>
#include <assert.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

yasp::GPUResourceManagerD3D::GPUResourceManagerD3D(RenderContextD3D * renderContext) : resourceCounter(0)
{
	this->device = renderContext->Device();
	this->deviceContext = renderContext->DeviceContext();
}

yasp::GPUResourceManagerD3D::~GPUResourceManagerD3D()
{
	for (auto& r : resourceMap)
	{
		switch (r.second.type)
		{
		case ResourceType::BUFFER:
			r.second.buffer->Release();
			break;
		case ResourceType::VERTEX_SHADER:
			r.second.vertexShader->Release();
			break;
		case ResourceType::GEOMETRY_SHADER:
			r.second.geometryShader->Release();
			break;
		case ResourceType::PIXEL_SHADER:
			r.second.pixelShader->Release();
			break;
		case ResourceType::COMPUTE_SHADER:
			r.second.computeShader->Release();
			break;
		case ResourceType::BLEND_STATE:
			r.second.blendState->Release();
			break;
		case ResourceType::DEPTH_STENCIL_STATE:
			r.second.depthStencilState->Release();
			break;
		case ResourceType::DEPTH_STENCIL_VIEW:
			r.second.depthStencilView->Release();
			break;
		case ResourceType::INPUT_LAYOUT:
			r.second.inputLayout->Release();
			break;
		case ResourceType::TEXTURE_VIEW:
			r.second.shaderResourceView->Release();
			break;
		}
		
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

	GPUResourceID id(resourceCounter++);
	resourceMap[id] = { ResourceType::BUFFER, buffer };
	return id;
}

yasp::GPUResourceID yasp::GPUResourceManagerD3D::CreateVertexShader(const std::string & filename)
{
	ID3DBlob* shaderCode = nullptr;
	ID3DBlob* errors = nullptr;
	HRESULT hr = D3DCompileFromFile(std::wstring(filename.begin(), filename.end()).c_str(),
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&shaderCode,
		&errors);
	assert(SUCCEEDED(hr));
	ShaderReflection(shaderCode);
	ID3D11VertexShader* vertexShader;
	hr = device->CreateVertexShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), nullptr, &vertexShader);
	assert(SUCCEEDED(hr));
	shaderCode->Release();

	GPUResourceID id(resourceCounter++);
	resourceMap[id] = { ResourceType::VERTEX_SHADER, vertexShader };
	return id;
}

void yasp::GPUResourceManagerD3D::ShaderReflection(ID3DBlob * shaderByteCode)
{
}
