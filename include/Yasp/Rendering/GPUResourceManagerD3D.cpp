#include <Yasp/Rendering/GPUResourceManagerD3D.h>
#include <assert.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <vector>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

const int32 textureFormatMap[] = {
	(int32)DXGI_FORMAT_R32G32B32A32_TYPELESS,
	(int32)DXGI_FORMAT_R32G32B32_TYPELESS,
	(int32)DXGI_FORMAT_R32G32_TYPELESS,
	(int32)DXGI_FORMAT_R32_TYPELESS,
	(int32)DXGI_FORMAT_R32G32B32A32_FLOAT,
	(int32)DXGI_FORMAT_R32G32B32_FLOAT,
	(int32)DXGI_FORMAT_R32G32_FLOAT,
	(int32)DXGI_FORMAT_R32_FLOAT,
	(int32)DXGI_FORMAT_R32G32B32A32_SINT,
	(int32)DXGI_FORMAT_R32G32B32_SINT,
	(int32)DXGI_FORMAT_R32G32_SINT,
	(int32)DXGI_FORMAT_R32_SINT,
	(int32)DXGI_FORMAT_R32G32B32A32_UINT,
	(int32)DXGI_FORMAT_R32G32B32_UINT,
	(int32)DXGI_FORMAT_R32G32_UINT,
	(int32)DXGI_FORMAT_R32_UINT,
	(int32)DXGI_FORMAT_R16G16B16A16_TYPELESS,
	(int32)DXGI_FORMAT_R16G16B16A16_FLOAT,
	(int32)DXGI_FORMAT_R16G16B16A16_UNORM,
	(int32)DXGI_FORMAT_R16G16B16A16_SNORM,
	(int32)DXGI_FORMAT_R16G16B16A16_UINT,
	(int32)DXGI_FORMAT_R16G16B16A16_SINT,
	(int32)DXGI_FORMAT_R24G8_TYPELESS,
	(int32)DXGI_FORMAT_D24_UNORM_S8_UINT,
	(int32)DXGI_FORMAT_R8G8B8A8_UINT,
	(int32)DXGI_FORMAT_R8G8B8A8_SINT,
	(int32)DXGI_FORMAT_R8G8B8A8_UNORM,
	(int32)DXGI_FORMAT_R8G8B8A8_SNORM,
};

yasp::GPUResourceManagerD3D::GPUResourceManagerD3D(RenderContextD3D * renderContext) : resourceCounter(1)
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
			delete (Shader*)r.second.resourceData;
			r.second.resourceData = nullptr;
			break;
		case ResourceType::GEOMETRY_SHADER:
			r.second.geometryShader->Release();
			delete (Shader*)r.second.resourceData;
			r.second.resourceData = nullptr;
			break;
		case ResourceType::PIXEL_SHADER:
			r.second.pixelShader->Release();
			delete (Shader*)r.second.resourceData;
			r.second.resourceData = nullptr;
			break;
		case ResourceType::COMPUTE_SHADER:
			r.second.computeShader->Release();
			delete (Shader*)r.second.resourceData;
			r.second.resourceData = nullptr;
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
		case ResourceType::RASTERIZER:
			r.second.rasterizerState->Release();
			break;
		case ResourceType::TEXTURE2D:
			r.second.texture2D->Release();
			break;
		}
		delete[] r.second.resourceData;
	}
}

ID3D11Buffer * yasp::GPUResourceManagerD3D::CreateInternalBuffer(const BufferDesc & bufferDesc, void * initialData)
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
	bd.BindFlags |= bufferDesc.bind & BufferBinding::BIND_SHADER_BUFFER ? D3D11_BIND_CONSTANT_BUFFER : 0U;
	bd.BindFlags |= bufferDesc.bind & BufferBinding::BIND_STREAM_OUT ? D3D11_BIND_STREAM_OUTPUT : 0U;
	bd.BindFlags |= bufferDesc.bind & BufferBinding::BIND_COMPUTE_ACCESS ? D3D11_BIND_UNORDERED_ACCESS : 0U;

	bd.CPUAccessFlags |= bufferDesc.usage == Usage::CPU_READ_WRITE ? D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE : 0U;
	bd.CPUAccessFlags |= bufferDesc.usage == Usage::GPU_READ_CPU_WRITE ? D3D11_CPU_ACCESS_WRITE : 0U;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = initialData;

	ID3D11Buffer* buffer;

	HRESULT hr = device->CreateBuffer(&bd, initialData ? &sd : nullptr, &buffer);
	assert(SUCCEEDED(hr));
	return buffer;
}

yasp::GPUBuffer yasp::GPUResourceManagerD3D::CreateBuffer(const BufferDesc& bufferDesc, void * initialData)
{
	auto buffer = CreateInternalBuffer(bufferDesc, initialData);
	void* bufferData = new uint8_t[bufferDesc.size];
	GPUBuffer id(resourceCounter++, this);
	resourceMap[id] = { ResourceType::BUFFER, buffer, bufferData, static_cast<size_t>(bufferDesc.size) };
	return id;
}

yasp::Shader yasp::GPUResourceManagerD3D::CreateVertexShader(const void * shaderSourceCode, size_t shaderSourceSize)
{
	ID3DBlob* shaderCode = nullptr;
	ID3DBlob* errors = nullptr;
	HRESULT hr = D3DCompile(shaderSourceCode, shaderSourceSize, nullptr, nullptr, nullptr, "main", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &shaderCode, &errors);
	if (FAILED(hr))
	{
		if (errors != nullptr)
		{
			auto exception = std::exception(static_cast<char*>(errors->GetBufferPointer()));
			errors->Release();
			throw exception;
		}
		throw std::exception("Failed to create vertex shader");
	}
	
	ID3D11VertexShader* vertexShader;
	hr = device->CreateVertexShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(hr))
	{
		shaderCode->Release();
		throw std::exception("Failed to reflect shader");
	}
	ShaderD3D* shader = new ShaderD3D(ShaderType::VERTEX, this);
	Shader* id = new Shader(resourceCounter++, shader);
	resourceMap[*id] = { ResourceType::VERTEX_SHADER, vertexShader, id, sizeof(*id) };
	VertexShaderReflection(shaderCode, *shader);
	shaderCode->Release();
	return *id;
}

yasp::Shader yasp::GPUResourceManagerD3D::CreateVertexShader(const std::string & filename)
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

	ID3D11VertexShader* vertexShader;
	hr = device->CreateVertexShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), nullptr, &vertexShader);

	assert(SUCCEEDED(hr));
	ShaderD3D* shader = new ShaderD3D(ShaderType::VERTEX, this);
	Shader* id = new Shader(resourceCounter++, shader);
	resourceMap[*id] = { ResourceType::VERTEX_SHADER, vertexShader, id, sizeof(*id) };
	VertexShaderReflection(shaderCode, *shader);
	shaderCode->Release();
	return *id;
}

yasp::Shader yasp::GPUResourceManagerD3D::CreatePixelShader(const std::string & filename)
{
	ID3DBlob* shaderCode = nullptr;
	ID3DBlob* errors = nullptr;
	HRESULT hr = D3DCompileFromFile(std::wstring(filename.begin(), filename.end()).c_str(),
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&shaderCode,
		&errors);
	//OutputDebugStringA((char*)errors->GetBufferPointer());
	assert(SUCCEEDED(hr));
	

	ID3D11PixelShader* pixelShader;
	hr = device->CreatePixelShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), nullptr, &pixelShader);
	assert(SUCCEEDED(hr));
	
	
	ShaderD3D* shader = new ShaderD3D(ShaderType::PIXEL, this);
	Shader* id = new Shader(resourceCounter++, shader);
	resourceMap[*id] = { ResourceType::PIXEL_SHADER, pixelShader, id, sizeof(*id) };
	PixelShaderReflection(shaderCode, *shader);
	shaderCode->Release();
	return *id;
}

yasp::Shader yasp::GPUResourceManagerD3D::CreatePixelShader(const void * shaderSourceCode, size_t shaderSourceSize)
{
	ID3DBlob* shaderCode = nullptr;
	ID3DBlob* errors = nullptr;
	HRESULT hr = D3DCompile(shaderSourceCode, shaderSourceSize, nullptr, nullptr, nullptr, "main", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &shaderCode, &errors);
	if (FAILED(hr))
	{
		throw std::exception(static_cast<char*>(errors->GetBufferPointer()));
	}

	ID3D11PixelShader* pixelShader;
	hr = device->CreatePixelShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), nullptr, &pixelShader);
	if (FAILED(hr))
	{
		shaderCode->Release();
		throw std::exception("Failed to reflect shader");
	}
	ShaderD3D* shader = new ShaderD3D(ShaderType::PIXEL, this);
	Shader* id = new Shader(resourceCounter++, shader);
	resourceMap[*id] = { ResourceType::VERTEX_SHADER, pixelShader, id, sizeof(*id) };
	PixelShaderReflection(shaderCode, *shader);
	shaderCode->Release();
	return *id;
}

yasp::Shader yasp::GPUResourceManagerD3D::GetShader(const GPUResourceID & id)
{
	auto it = resourceMap.find(id);
	assert(it != resourceMap.end());
	return *reinterpret_cast<Shader*>(it->second.resourceData);
}

yasp::GPUResourceID yasp::GPUResourceManagerD3D::CreateRasterizer(RasterizerDesc rasterizerDesc)
{
	ID3D11RasterizerState* rs;
	D3D11_RASTERIZER_DESC rd;
	switch (rasterizerDesc.cullMode)
	{
	case CullMode::BACK:
		rd.CullMode = D3D11_CULL_BACK;
		break;
	case CullMode::FRONT:
		rd.CullMode = D3D11_CULL_FRONT;
		break;
	case CullMode::NONE:
		rd.CullMode = D3D11_CULL_NONE;
		break;
	}
	switch (rasterizerDesc.fillMode)
	{
	case FillMode::SOLID:
		rd.FillMode = D3D11_FILL_SOLID;
		break;
	case FillMode::WIREFRAME:
		rd.FillMode = D3D11_FILL_WIREFRAME;
		break;
	}
	switch (rasterizerDesc.windingOrder)
	{
	case WindingOrder::CLOCKWISE:
		rd.FrontCounterClockwise = false;
		break;
	case WindingOrder::COUNTERCLOCKWISE:
		rd.FrontCounterClockwise = true;
		break;
	}
	rd.AntialiasedLineEnable = false;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.DepthClipEnable = rasterizerDesc.enableDepthClip;
	rd.MultisampleEnable = false;
	rd.SlopeScaledDepthBias = 0;
	rd.ScissorEnable = rasterizerDesc.enableScissor;

	HRESULT hr = device->CreateRasterizerState(&rd, &rs);
	assert(SUCCEEDED(hr));
	GPUResourceID id(resourceCounter++);
	resourceMap[id] = { ResourceType::RASTERIZER, rs };
	return id;
}

yasp::GPUResourceID yasp::GPUResourceManagerD3D::CreateTexture2D(const Texture2DDesc & textureDesc, void * data)
{
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = textureDesc.width;
	td.Height = textureDesc.height;
	td.ArraySize = textureDesc.arraySize;
	td.MipLevels = textureDesc.mipLevels;
	td.SampleDesc.Count = textureDesc.sampleCount;
	td.SampleDesc.Quality = textureDesc.sampleQuality;
	switch (textureDesc.usage)
	{
	case Usage::CPU_READ_WRITE:
		td.Usage = D3D11_USAGE_STAGING;
		td.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		break;
	case Usage::GPU_READ:
		td.Usage = D3D11_USAGE_IMMUTABLE;
		break;
	case Usage::GPU_READ_CPU_WRITE:
		td.Usage = D3D11_USAGE_DYNAMIC;
		td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case Usage::GPU_READ_WRITE:
		td.Usage = D3D11_USAGE_DEFAULT;
		break;
	}

	td.BindFlags |= textureDesc.bind & TextureBind::TEXTURE_BIND_SHADER_VIEW ? D3D11_BIND_SHADER_RESOURCE : 0U;
	td.BindFlags |= textureDesc.bind & TextureBind::TEXTURE_BIND_RENDER_TARGET ? D3D11_BIND_RENDER_TARGET : 0U;
	td.BindFlags |= textureDesc.bind & TextureBind::TEXTURE_BIND_DEPTH_TARGET ? D3D11_BIND_DEPTH_STENCIL : 0U;

	td.Format = (DXGI_FORMAT)textureFormatMap[(int32)textureDesc.format];
	
	ID3D11Texture2D* texture;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;
	sd.SysMemPitch = 4 * td.Width;

	HRESULT hr = device->CreateTexture2D(&td, &sd, &texture);
	assert(SUCCEEDED(hr));
	auto id = GPUResourceID(resourceCounter++);
	resourceMap[id] = { ResourceType::TEXTURE2D, texture };
	return id;
}

yasp::GPUResourceID yasp::GPUResourceManagerD3D::CreateTexture2DView(const Texture2DViewDesc & textureViewDesc, const GPUResourceID & texture)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = (DXGI_FORMAT)textureFormatMap[(int32)textureViewDesc.format];
	srvd.Texture2D.MipLevels = textureViewDesc.mipLevels;
	srvd.Texture2D.MostDetailedMip = textureViewDesc.mostDetailedMip;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	

	ID3D11ShaderResourceView* srv;
	HRESULT hr = device->CreateShaderResourceView(resourceMap[texture].texture2D, &srvd, &srv);
	assert(SUCCEEDED(hr));
	auto id = GPUResourceID(resourceCounter++);
	resourceMap[id] = { ResourceType::TEXTURE_VIEW, srv };
	return id;
}

yasp::GPUResourceID yasp::GPUResourceManagerD3D::CreateSampler(const SamplerDesc & samplerDesc)
{
	D3D11_SAMPLER_DESC sd = {};
	const int32 samplerMap[] = {
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_BORDER
	};

	const int32 comparisonMap[] = {
		D3D11_COMPARISON_NEVER,
		D3D11_COMPARISON_LESS,
		D3D11_COMPARISON_EQUAL,
		D3D11_COMPARISON_LESS_EQUAL,
		D3D11_COMPARISON_GREATER,
		D3D11_COMPARISON_NOT_EQUAL,
		D3D11_COMPARISON_GREATER_EQUAL,
		D3D11_COMPARISON_ALWAYS
	};
	
	sd.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)samplerMap[(int32)samplerDesc.wrapModeU];
	sd.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)samplerMap[(int32)samplerDesc.wrapModeV];
	sd.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)samplerMap[(int32)samplerDesc.wrapModeW];
	sd.BorderColor[0] = samplerDesc.borderColor.x;
	sd.BorderColor[1] = samplerDesc.borderColor.y;
	sd.BorderColor[2] = samplerDesc.borderColor.z;
	sd.BorderColor[3] = samplerDesc.borderColor.w;
	sd.ComparisonFunc = (D3D11_COMPARISON_FUNC)comparisonMap[(int32)samplerDesc.comparisonFunc];
	switch (samplerDesc.filter)
	{
	case TextureFilter::ANISOTROPIC:
		sd.Filter = D3D11_FILTER_ANISOTROPIC;
		break;
	case TextureFilter::POINT:
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case TextureFilter::BILINEAR:
		sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		break;
	case TextureFilter::TRILINEAR:
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	}
	sd.MaxAnisotropy = samplerDesc.maxAnisotropy;
	sd.MaxLOD = samplerDesc.maxLOD;
	sd.MinLOD = samplerDesc.minLOD;
	sd.MipLODBias = samplerDesc.mipLODBias;

	ID3D11SamplerState* sampler;
	HRESULT hr = device->CreateSamplerState(&sd, &sampler);
	assert(SUCCEEDED(hr));
	auto id = GPUResourceID(resourceCounter++);
	resourceMap[id] = { ResourceType::TEXTURE_VIEW, sampler };
	return id;
}

yasp::GPUResourceID yasp::GPUResourceManagerD3D::CreateDepthStencilState(const DepthStencilDesc & depthStencilDesc)
{
	D3D11_DEPTH_STENCIL_DESC dsd;
	dsd.DepthEnable = depthStencilDesc.depthTest;
	dsd.StencilEnable = depthStencilDesc.stencilTest;
	dsd.StencilWriteMask = depthStencilDesc.stencilWriteMask;
	dsd.StencilReadMask = depthStencilDesc.stencilReadMask;
	switch (depthStencilDesc.depthWriteMask)
	{
		case DepthWriteMask::ZERO: { dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; break; };
		case DepthWriteMask::ALL: { dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; break; };						   
	}

	auto getComp = [](yasp::ComparisonFunc func) -> D3D11_COMPARISON_FUNC {
		switch (func)
		{
			case ComparisonFunc::NEVER: { return D3D11_COMPARISON_NEVER; };
			case ComparisonFunc::LESS: { return D3D11_COMPARISON_LESS; };
			case ComparisonFunc::EQUAL: { return D3D11_COMPARISON_EQUAL; };
			case ComparisonFunc::LESS_EQUAL: { return D3D11_COMPARISON_LESS_EQUAL; };
			case ComparisonFunc::GREATER: { return D3D11_COMPARISON_GREATER; };
			case ComparisonFunc::NOT_EQUAL: { return D3D11_COMPARISON_NOT_EQUAL; };
			case ComparisonFunc::GREATER_EQUAL: { return D3D11_COMPARISON_GREATER_EQUAL; };
			case ComparisonFunc::ALWAYS: { return D3D11_COMPARISON_ALWAYS; };
		}
		return D3D11_COMPARISON_ALWAYS;
	};
	dsd.DepthFunc = getComp(depthStencilDesc.comparisonMode);
	
	auto getOp = [](yasp::StencilOp op) -> D3D11_STENCIL_OP {
		switch (op)
		{
			case StencilOp::KEEP: return D3D11_STENCIL_OP_KEEP;
			case StencilOp::ZERO: return D3D11_STENCIL_OP_ZERO;
			case StencilOp::REPLACE: return D3D11_STENCIL_OP_REPLACE;
			case StencilOp::INCR_SAT: return D3D11_STENCIL_OP_INCR_SAT;
			case StencilOp::DECR_SAT: return D3D11_STENCIL_OP_DECR_SAT;
			case StencilOp::INVERT: return D3D11_STENCIL_OP_INVERT;
			case StencilOp::INCR: return D3D11_STENCIL_OP_INCR;
			case StencilOp::DECR: return D3D11_STENCIL_OP_DECR;
		}
		return D3D11_STENCIL_OP_KEEP;
	};

	dsd.FrontFace = {
		getOp(depthStencilDesc.onFrontFaceFail),
		getOp(depthStencilDesc.onFrontFaceFail),
		getOp(depthStencilDesc.onFrontFaceFail),
		getComp(depthStencilDesc.frontFaceComparison)
	};

	dsd.BackFace = {
		getOp(depthStencilDesc.onBackFaceFail),
		getOp(depthStencilDesc.onBackFaceFail),
		getOp(depthStencilDesc.onBackFaceFail),
		getComp(depthStencilDesc.backFaceComparison)
	};

	ID3D11DepthStencilState* depthStencilState = nullptr;
	HRESULT hr = device->CreateDepthStencilState(&dsd, &depthStencilState);
	assert(SUCCEEDED(hr));
	auto id = GPUResourceID(resourceCounter++);
	resourceMap[id] = { ResourceType::DEPTH_STENCIL_STATE, depthStencilState };
	return id;
}

yasp::GPUResourceID yasp::GPUResourceManagerD3D::CreateBlendState(const BlendStateDesc & blendStateDesc)
{
	D3D11_BLEND_DESC bd;
	bd.AlphaToCoverageEnable = blendStateDesc.alphaToCoverage;
	bd.IndependentBlendEnable = blendStateDesc.independentBlending;

	const auto getFactor = [](yasp::BlendFactor bf) -> D3D11_BLEND {
		switch (bf)
		{
			case yasp::BlendFactor::ONE: return D3D11_BLEND_ONE;
			case yasp::BlendFactor::ZERO: return D3D11_BLEND_ZERO;
			case yasp::BlendFactor::SRC_COLOR: return D3D11_BLEND_SRC_COLOR;
			case yasp::BlendFactor::SRC_ALPHA: return D3D11_BLEND_SRC_ALPHA;
			case yasp::BlendFactor::INV_SRC_COLOR: return D3D11_BLEND_INV_SRC_COLOR;
			case yasp::BlendFactor::INV_SRC_ALPHA: return D3D11_BLEND_INV_SRC_ALPHA;
			case yasp::BlendFactor::DEST_COLOR: return D3D11_BLEND_DEST_COLOR;
			case yasp::BlendFactor::DEST_ALPHA: return D3D11_BLEND_DEST_ALPHA;
			case yasp::BlendFactor::INV_DEST_COLOR: return D3D11_BLEND_INV_DEST_COLOR;
			case yasp::BlendFactor::INV_DEST_ALPHA: return D3D11_BLEND_INV_DEST_ALPHA;
			default: return D3D11_BLEND_ONE;
		}
	};

	const auto getOp = [](yasp::BlendOp bo) -> D3D11_BLEND_OP {
		switch (bo)
		{
			case yasp::BlendOp::ADD: return D3D11_BLEND_OP_ADD;
			case yasp::BlendOp::SUBTRACT: return D3D11_BLEND_OP_SUBTRACT;
			case yasp::BlendOp::MIN: return D3D11_BLEND_OP_MIN;
			case yasp::BlendOp::MAX: return D3D11_BLEND_OP_MAX;
			default: return D3D11_BLEND_OP_ADD;
		}
	};

	for (size_t i = 0; i < 8; i++)
	{
		bd.RenderTarget[i].BlendEnable = blendStateDesc.renderTargetBlend[i].enable;
		bd.RenderTarget[i].SrcBlend = getFactor(blendStateDesc.renderTargetBlend[i].srcBlend);
		bd.RenderTarget[i].DestBlend = getFactor(blendStateDesc.renderTargetBlend[i].destBlend);
		bd.RenderTarget[i].SrcBlendAlpha = getFactor(blendStateDesc.renderTargetBlend[i].srcBlendAlpha);
		bd.RenderTarget[i].DestBlendAlpha = getFactor(blendStateDesc.renderTargetBlend[i].destBlendAlpha);
		bd.RenderTarget[i].BlendOp = getOp(blendStateDesc.renderTargetBlend[i].blendOpColor);
		bd.RenderTarget[i].BlendOpAlpha = getOp(blendStateDesc.renderTargetBlend[i].blendOpAlpha);
		bd.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	

	ID3D11BlendState* blendState = nullptr;
	HRESULT hr = device->CreateBlendState(&bd, &blendState);
	assert(SUCCEEDED(hr));
	auto id = GPUResourceID(resourceCounter++);
	resourceMap[id] = { ResourceType::BLEND_STATE, blendState };
	return id;

}

void yasp::GPUResourceManagerD3D::UpdateBuffer(const GPUResourceID& id, void * data, uint32 size)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		deviceContext->Map(f->second.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, data, size);
		deviceContext->Unmap(f->second.buffer, 0);
	}
}

void yasp::GPUResourceManagerD3D::UpdateBuffer(const GPUResourceID & id)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		deviceContext->Map(f->second.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, f->second.resourceData, f->second.resourceDataSize);
		deviceContext->Unmap(f->second.buffer, 0);
	}
}

void yasp::GPUResourceManagerD3D::StageBuffer(const GPUResourceID & id, void * data, size_t size, size_t offset)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		memcpy((uint8_t*)f->second.resourceData + offset, data, size);
	}
}

yasp::AssignableMemory yasp::GPUResourceManagerD3D::GetBufferSegment(const GPUResourceID & id, const std::string & identifier)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		if (auto g = f->second.namedOffsets.find(identifier); g != f->second.namedOffsets.end())
		{
			return AssignableMemory(static_cast<uint8_t*>(f->second.resourceData) + g->second.offset, g->second.size);
		}
		return AssignableMemory(f->second.resourceData, f->second.resourceDataSize);
	}
	return AssignableMemory();
}

size_t yasp::GPUResourceManagerD3D::GetBufferElementCount(const GPUResourceID & id)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		return f->second.names.size();
	}
	return 0;
}

size_t yasp::GPUResourceManagerD3D::GetBufferSize(const GPUResourceID & id) const
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		return f->second.resourceDataSize;
	}
	return 0;
}

const std::string & yasp::GPUResourceManagerD3D::GetBufferElementName(const GPUResourceID & id, size_t offset)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		return f->second.names[offset];
	}
	throw std::exception("GetBufferElementName failed to find resource");
}

yasp::AssignableMemory yasp::GPUResourceManagerD3D::GetBufferSegment(const GPUResourceID & id, size_t offset)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		return AssignableMemory(static_cast<uint8_t*>(f->second.resourceData) + f->second.offsets[offset].offset, f->second.offsets[offset].size);
	}
	throw std::exception("GetBufferSegment GPU resource ID not found");
}

void yasp::GPUResourceManagerD3D::PushState()
{
	backups.push_back({});
	D3DStateBackup& backup = backups.back();
	deviceContext->IAGetVertexBuffers(0, 8, backup.vertexBuffers, backup.vertexBufferStrides, backup.vertexBufferOffsets);
	deviceContext->IAGetPrimitiveTopology(&backup.topology);
	deviceContext->IAGetInputLayout(&backup.inputLayout);
	deviceContext->IAGetIndexBuffer(&backup.indexBuffer, &backup.indexFormat, &backup.indexOffset);

	deviceContext->VSGetConstantBuffers(0, 8, backup.vsBuffers);
	deviceContext->VSGetSamplers(0, 8, backup.vsSamplers);
	deviceContext->VSGetShader(&backup.vshader, backup.vsClassInstances, &backup.vsClassInstanceCount);
	deviceContext->VSGetShaderResources(0, 8, backup.vsResources);
	// TODO Hull, Domain, Geometry, Compute, UAVS

	deviceContext->RSGetState(&backup.rasterizer);
	deviceContext->RSGetScissorRects(&backup.scissorRectsCount, backup.scissorRects);
	deviceContext->RSGetViewports(&backup.viewPortsCount, backup.viewPorts);

	deviceContext->PSGetShader(&backup.pshader, backup.vsClassInstances, &backup.psClassInstanceCount);
	deviceContext->PSGetConstantBuffers(0, 8, backup.psBuffers);
	deviceContext->PSGetSamplers(0, 8, backup.psSamplers);
	deviceContext->PSGetShaderResources(0, 8, backup.psResources);

	deviceContext->OMGetBlendState(&backup.blendState, backup.blendFactor, &backup.sampleMask);
	deviceContext->OMGetDepthStencilState(&backup.depthStencilState, &backup.stencilRef);
	deviceContext->OMGetRenderTargets(8, backup.renderTargetViews, &backup.depthStencilView);
}

void yasp::GPUResourceManagerD3D::PopState()
{
	D3DStateBackup& backup = backups.back();
	deviceContext->IASetVertexBuffers(0, 8, backup.vertexBuffers, backup.vertexBufferStrides, backup.vertexBufferOffsets);
	deviceContext->IASetPrimitiveTopology(backup.topology);
	deviceContext->IASetInputLayout(backup.inputLayout);
	deviceContext->IASetIndexBuffer(backup.indexBuffer, backup.indexFormat, backup.indexOffset);

	deviceContext->VSSetConstantBuffers(0, 8, backup.vsBuffers);
	deviceContext->VSSetSamplers(0, 8, backup.vsSamplers);
	deviceContext->VSSetShaderResources(0, 8, backup.vsResources);
	deviceContext->VSSetShader(backup.vshader, backup.vsClassInstances, backup.vsClassInstanceCount);

	deviceContext->RSSetState(backup.rasterizer);
	deviceContext->RSSetScissorRects(backup.scissorRectsCount, backup.scissorRects);
	deviceContext->RSSetViewports(backup.viewPortsCount, backup.viewPorts);

	deviceContext->PSSetShader(backup.pshader, backup.psClassInstances, backup.psClassInstanceCount);
	deviceContext->PSSetConstantBuffers(0, 8, backup.psBuffers);
	deviceContext->PSSetShaderResources(0, 8, backup.psResources);
	deviceContext->PSSetSamplers(0, 8, backup.psSamplers);

	deviceContext->OMSetBlendState(backup.blendState, backup.blendFactor, backup.sampleMask);
	deviceContext->OMSetDepthStencilState(backup.depthStencilState, backup.stencilRef);
	deviceContext->OMSetRenderTargets(8, backup.renderTargetViews, backup.depthStencilView);
	
	backups.pop_back();
}

void yasp::GPUResourceManagerD3D::SetVertexBuffer(const GPUResourceID& id, uint32 stride, uint32 offset)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		deviceContext->IASetVertexBuffers(0, 1, &f->second.buffer, &stride, &offset);
	}
}

void yasp::GPUResourceManagerD3D::SetIndexBuffer(const GPUResourceID & id, IndexFormat format, uint32 offset)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		deviceContext->IASetIndexBuffer(f->second.buffer, format == IndexFormat::UINT16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, offset);
	}
}

void yasp::GPUResourceManagerD3D::SetShaderBuffers(ShaderType shader, GPUResourceID * buffers, uint32 startSlot, uint32 count)
{
	std::vector<ID3D11Buffer*> d3dBuffers(count,nullptr);
	for (uint32 i = 0; i < count; i++)
	{
		if (auto f = resourceMap.find(buffers[i]); f != resourceMap.end())
		{
			d3dBuffers[i] = f->second.buffer;
		}
		else
		{
			d3dBuffers[i] = nullptr;
		}
	}

	switch (shader)
	{
	case ShaderType::VERTEX:
		deviceContext->VSSetConstantBuffers(startSlot, count, d3dBuffers.data());
		break;
	case ShaderType::GEOMETRY:
		deviceContext->GSSetConstantBuffers(startSlot, count, d3dBuffers.data());
		break;
	case ShaderType::PIXEL:
		deviceContext->PSSetConstantBuffers(startSlot, count, d3dBuffers.data());
		break;
	case ShaderType::COMPUTE:
		deviceContext->CSSetConstantBuffers(startSlot, count, d3dBuffers.data());
		break;
	}
}

void yasp::GPUResourceManagerD3D::SetShaderTextureViews(ShaderType shader, GPUResourceID * textureViews, uint32 startSlot, uint32 count)
{
	std::vector<ID3D11ShaderResourceView*> srvs(count, nullptr);
	for (uint32 i = 0; i < count; i++)
	{
		if (auto f = resourceMap.find(textureViews[i]); f != resourceMap.end())
		{
			srvs[i] = f->second.shaderResourceView;
		}
		else
		{
			srvs[i] = nullptr;
		}
	}

	switch (shader)
	{
	case ShaderType::VERTEX:
		deviceContext->VSSetShaderResources(startSlot, count, srvs.data());
		break;
	case ShaderType::GEOMETRY:
		deviceContext->GSSetShaderResources(startSlot, count, srvs.data());
		break;
	case ShaderType::PIXEL:
		deviceContext->PSSetShaderResources(startSlot, count, srvs.data());
		break;
	case ShaderType::COMPUTE:
		deviceContext->CSSetShaderResources(startSlot, count, srvs.data());
		break;
	}
}

void yasp::GPUResourceManagerD3D::SetShaderSamplers(ShaderType shader, GPUResourceID * samplers, uint32 startSlot, uint32 count)
{
	std::vector<ID3D11SamplerState*> d3dsamplers(count, nullptr);
	for (uint32 i = 0; i < count; i++)
	{
		if (auto f = resourceMap.find(samplers[i]); f != resourceMap.end())
		{
			d3dsamplers[i] = f->second.sampler;
		}
		else
		{
			d3dsamplers[i] = nullptr;
		}
	}

	switch (shader)
	{
	case ShaderType::VERTEX:
		deviceContext->VSSetSamplers(startSlot, count, d3dsamplers.data());
		break;
	case ShaderType::GEOMETRY:
		deviceContext->GSSetSamplers(startSlot, count, d3dsamplers.data());
		break;
	case ShaderType::PIXEL:
		deviceContext->PSSetSamplers(startSlot, count, d3dsamplers.data());
		break;
	case ShaderType::COMPUTE:
		deviceContext->CSSetSamplers(startSlot, count, d3dsamplers.data());
		break;
	}
}

void yasp::GPUResourceManagerD3D::SetVertexShader(const GPUResourceID& id)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		deviceContext->VSSetShader(f->second.vertexShader, nullptr, 0);
		if (auto il = resourceMap.find(id.ID() + 1); il != resourceMap.end())
		{
			deviceContext->IASetInputLayout(il->second.inputLayout);
		}
	}
}

void yasp::GPUResourceManagerD3D::SetGeometryShader(const GPUResourceID & id)
{
}

void yasp::GPUResourceManagerD3D::SetPixelShader(const GPUResourceID& id)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		deviceContext->PSSetShader(f->second.pixelShader, nullptr, 0);
	}
}

void yasp::GPUResourceManagerD3D::SetRasterizer(const GPUResourceID& id)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		deviceContext->RSSetState(f->second.rasterizerState);
	}
}

void yasp::GPUResourceManagerD3D::SetDepthStencilState(const GPUResourceID & id, uint32_t reference)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		deviceContext->OMSetDepthStencilState(f->second.depthStencilState, reference);
	}
}

void yasp::GPUResourceManagerD3D::SetBlendState(const GPUResourceID & id, const float * blendFactor, uint32 mask)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		deviceContext->OMSetBlendState(f->second.blendState, blendFactor, mask);
	}
}

void yasp::GPUResourceManagerD3D::PixelShaderReflection(ID3DBlob * shaderByteCode, ShaderD3D & shader)
{
	ID3D11ShaderReflection* reflection;
	HRESULT hr = D3DReflect(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
	assert(SUCCEEDED(hr));

	D3D11_SHADER_DESC shaderDesc;
	reflection->GetDesc(&shaderDesc);
	RegisterResourceBindings(shaderDesc, reflection, shader);
	reflection->Release();
}

void yasp::GPUResourceManagerD3D::VertexShaderReflection(ID3DBlob * shaderCode, ShaderD3D& shader)
{
	ID3D11ShaderReflection* reflection;
	ID3D11InputLayout* inputLayout = nullptr;

	HRESULT hr = D3DReflect(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
	assert(SUCCEEDED(hr));

	D3D11_SHADER_DESC shaderDesc;
	reflection->GetDesc(&shaderDesc);

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs;
	uint32_t offset = 0;
	for (uint32_t i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC signatureParamaterDesc;
		reflection->GetInputParameterDesc(i, &signatureParamaterDesc);
		D3D11_INPUT_ELEMENT_DESC inputElementDesc;
		inputElementDesc.SemanticName = signatureParamaterDesc.SemanticName;
		inputElementDesc.SemanticIndex = signatureParamaterDesc.SemanticIndex;
		inputElementDesc.AlignedByteOffset = offset;
		inputElementDesc.InputSlot = 0;
		inputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElementDesc.InstanceDataStepRate = 0;
		const std::string semName(inputElementDesc.SemanticName);
		if (signatureParamaterDesc.Mask == 1)
		{
			if (semName == "SV_InstanceID")
				continue;
			if (semName == "SV_VertexID")
				continue;
			if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			else if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32_UINT;
			offset += 4;

		}
		else if (signatureParamaterDesc.Mask <= 3)
		{
			if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			else if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			offset += 8;
		}
		else if (signatureParamaterDesc.Mask <= 7)
		{
			if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			else if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			offset += 12;
		}
		else if (signatureParamaterDesc.Mask <= 15)
		{
			if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				if(semName != "COLOR")
					inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				else
					inputElementDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			else if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (signatureParamaterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			offset += 16;
		}
		inputElementDescs.push_back(inputElementDesc);
	}
	if (inputElementDescs.size() > 0)
	{
		hr = device->CreateInputLayout(inputElementDescs.data(), inputElementDescs.size(), shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), &inputLayout);
		assert(SUCCEEDED(hr));
	}
	GPUResourceID id(resourceCounter++);
	resourceMap[id] = { ResourceType::INPUT_LAYOUT, inputLayout };
	RegisterResourceBindings(shaderDesc, reflection, shader);
	reflection->Release();
}

void yasp::GPUResourceManagerD3D::RegisterResourceBindings(const D3D11_SHADER_DESC& desc, ID3D11ShaderReflection* reflection, ShaderD3D & shader)
{
	for (unsigned int i = 0; i < desc.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC sibd;
		reflection->GetResourceBindingDesc(i, &sibd);
		if (sibd.Type == D3D_SIT_CBUFFER)
		{			
			ID3D11ShaderReflectionConstantBuffer* cb = reflection->GetConstantBufferByName(sibd.Name);
			D3D11_SHADER_BUFFER_DESC sbd;
			cb->GetDesc(&sbd);
			BufferDesc bd;
			bd.bind = BufferBinding::BIND_SHADER_BUFFER;
			bd.size = sbd.Size;
			bd.byteStride = 0;
			bd.usage = Usage::GPU_READ_CPU_WRITE;
			auto buffer = CreateInternalBuffer(bd, nullptr);
			GPUBuffer id(resourceCounter++, this);
			void* bufferData = new uint8_t[bd.size];

			resourceMap[id] = { ResourceType::BUFFER, buffer, bufferData, static_cast<size_t>(bd.size) };
			for (unsigned int j = 0; j < sbd.Variables; j++)
			{
				ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(j);
				D3D11_SHADER_VARIABLE_DESC svd;
				var->GetDesc(&svd);
				RegisterProperty(id, svd.Name, svd.Size, svd.StartOffset);
			}
			shader.RegisterBuffer(sibd.Name, id, sibd.BindPoint);
		}
		if (sibd.Type == D3D_SIT_TEXTURE)
		{
			shader.RegisterBinding(sibd.Name, ShaderResourceType::TEXTURE, sibd.BindPoint);
		}
		if (sibd.Type == D3D_SIT_SAMPLER)
		{
			shader.RegisterBinding(sibd.Name, ShaderResourceType::SAMPLER, sibd.BindPoint);
		}
	}
}

void yasp::GPUResourceManagerD3D::RegisterProperty(const GPUResourceID & id, const std::string & name, int32_t size, int32_t offset)
{
	if (auto f = resourceMap.find(id); f != resourceMap.end())
	{
		if (auto g = f->second.namedOffsets.find(name); g == f->second.namedOffsets.end())
		{
			f->second.namedOffsets[name] = { size, offset };
			f->second.names.push_back(name);
			f->second.offsets.push_back({ size, offset });
		}
	}
}
