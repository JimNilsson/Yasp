#include <Yasp/Window/Window.h>
#include <Yasp/Rendering/RenderContext.h>
#include <Yasp/Rendering/Descriptions.h>
#include <Yasp/EntityComponent/Entity.h>
#include <Yasp/EntityComponent/EntityManager.h>
#include <Yasp/Window/Keyboard.h>
#include <Yasp/Window/Mouse.h>
#include <Yasp/Timer.h>
#include <iostream>
#include <vector>
#include <Yasp/EntityComponent/Pool.h>
#include <utility>

yasp::float3 randvec()
{
	float r = (rand() % 100) / 100.0f;
	//if (r < 0.5)
	//{
	//	return { 1.0f, 0.0f, 0.0f };
	//}
	//return { 0.0f, 1.0f, 0.0f };
	float x = (rand() % 100) / 100.0f;
	float y = (rand() % 100) / 100.0f;
	float z = (rand() % 100) / 100.0f;
	return normalize(yasp::float3(x, y, z));
}

#include <functional>

struct Pos : public yasp::float3
{
	using yasp::float3::float3;
};
struct Velocity : public yasp::float3
{
	using yasp::float3::float3;
};

struct PointLight : public yasp::float4
{
	using yasp::float4::float4;
};

class A {
public:
	A() {}
	virtual ~A() {}
	virtual int operator[](size_t) { return 2; };
};

class B : public A
{
public:
	B() : A() {}
	~B() {}
	int operator[](size_t a) override final { return a; }
};

struct C
{
	C(int a) : c(a) {}
	int operator()(const C& self) { return self.c; }
	std::string operator[](const std::string& s) { return "asdf"; }
	int c;
};


#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <Yasp/stb_image.h>
int main(int argc, char** argv)
{
	A a;
	B b;
	A c = b;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	yasp::EntityManager em;
	yasp::float3 pos = { 0, 0, 0 };
	yasp::float3 forward = { 0,0,1 };
	yasp::float3 up = { 0,1,0 };
	yasp::Entity e1 = em.Create();
	yasp::Entity e2 = em.Create();
	yasp::Entity e3 = em.Create();
	yasp::Entity e4 = em.Create();
	yasp::Entity e5 = em.Create();
	std::cout << e1 << ", " << e2 << ", " << e3 << ", " << e4 << std::endl;

	em.Register(e1, Pos({ 1.0f, 2.0f, 3.0 }), Velocity({ 0.0f, 1.0f, 0.0f }));
	em.Register(e2, Pos({ 0.0f, 0.0f, 3.0 }), Velocity({ 1.0f, 0.0f, 0.0f }));
	em.Register(e3, Pos({ 0.0f, 0.0f, 3.0 }), Velocity({ 0.0f, 0.0f, 1.0f }));
	em.Register(e4, Pos({ 0.0f, 0.0f, 3.0 }), Velocity({ 0.5f, 0.5f, 0.0f }));

	//for (int i = 0; i < 100; i++)
	//{
	//	auto ent = em.Create();
	//	auto p = randvec().xyz * 2.0f;
	//	em.Register(ent, Pos(p.x, p.y, p.z), Velocity({ 1.0f, 0.0f, 0.0f }));
	//}
	


	yasp::Window window(1280, 720);
	yasp::RenderContext renderContext(window.GetWindowHandle());
	window.SetGrabCursor(true);
	yasp::BufferDesc bd;
	bd.bind = yasp::BIND_SHADER_BUFFER;
	bd.byteStride = 0;
	bd.size = 16 * sizeof(float);
	bd.usage = yasp::Usage::GPU_READ_CPU_WRITE;
	
	auto view = yasp::mat4::LookToLH({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
	auto projection = yasp::mat4::PerspectiveLH(yasp::PI<float> / 4, 1280.0f / 720.0f, 0.1f, 50.0f);
	yasp::mat4 matrix = ~(view * projection);

	PointLight pl = { 0.0f, 2.5f, 0.0f, 10.0f };

	struct PointLights {
		yasp::vec4<uint32_t> pointlightCount;
		PointLight pointlights[16];
	};
	PointLights pls;
	pls.pointlightCount.x = 4;
	for (auto& pl : pls.pointlights)
	{
		auto p = randvec() * 25.0f;
		p.y = 2.5f;
		pl = PointLight(p.x, p.y, p.z, 2.0f);
	}


	auto pipe = renderContext.ResourceManager();
	auto vshader = pipe->CreateVertexShader("SimpleVS.hlsl");
	auto pshader = pipe->CreatePixelShader("SimplePS.hlsl");
	//

	auto rasterizer = pipe->CreateRasterizer({
		yasp::FillMode::SOLID,
		yasp::CullMode::NONE,
		yasp::WindingOrder::CLOCKWISE
	});

	vshader.Bind();
	pshader.Bind();
	pipe->SetRasterizer(rasterizer);


	struct Vertex
	{
		yasp::float3 pos;
		yasp::float3 nor;
		yasp::float2 tex;
	};

	Vertex positions[14] = {
		Vertex({ yasp::float3(-1.0f, -1.0f, -1.0f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(1.0f, -1.0f, -1.0f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
		Vertex({ yasp::float3(1.0f, -1.0f,  1.0f), yasp::float3(0.0f,-1.0f,0.0f),yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(1.0f,  1.0f, -1.0f),	yasp::float3(1.0f,0.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(1.0f,  1.0f,  1.0f),	yasp::float3(1.0f,0.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-1.0f,  1.0f,  1.0f), yasp::float3(0.0f,1.0f,0.0f),yasp::float2(1.0f, 0.0f) }),
		Vertex({ yasp::float3(1.0f, -1.0f,  1.0f),	yasp::float3(0.0f,1.0f,0.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(-1.0f, -1.0f,  1.0f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-1.0f, -1.0f, -1.0f),yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-1.0f,  1.0f,  1.0f),yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
		Vertex({ yasp::float3(-1.0f,  1.0f, -1.0f),yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(1.0f,  1.0f, -1.0f),yasp::float3(-1.0f,0.0f,0.0f),	 yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-1.0f, -1.0f, -1.0f),yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(1.0f,0.0f) }),
		Vertex({ yasp::float3(1.0f, -1.0f, -1.0f),yasp::float3(-1.0f,0.0f,0.0f),	 yasp::float2(1.0f,1.0f) })
	};

	bd.bind = yasp::BIND_VERTEX_BUFFER;
	bd.size = 14 * sizeof(Vertex);
	bd.usage = yasp::Usage::GPU_READ_WRITE;
	bd.byteStride = 0;
	
	auto vbuffer = pipe->CreateBuffer(bd, positions);

	Vertex floor[4] = {
		Vertex({ yasp::float3(-15.5f, 0.0f, -15.5f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-15.5f, 0.0f, 15.5f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(30.0f, 0.0f) }),
		Vertex({ yasp::float3(15.5f, 0.0f, -15.5f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 30.0f) }),
		Vertex({ yasp::float3(15.5f, 0.0f, 15.5f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(30.0f, 30.0f) })
	};

	bd.size = 4 * sizeof(Vertex);
	auto floorbuffer = pipe->CreateBuffer(bd, floor);

	pipe->SetVertexBuffer(vbuffer, sizeof(Vertex), 0);

	renderContext.SetTopology(yasp::Topology::TRIANGLE_STRIP);
	renderContext.SetViewport({
		0.0f,
		0.0f,
		1280.0f,
		720.0f,
		0.0f,
		1.0f
	});

	int imw, imh, imc;
	auto imdata = stbi_load("floor2.png", &imw, &imh, &imc, 4);

	yasp::Texture2DDesc td;
	td.arraySize = 1;
	td.bind = yasp::TextureBind::TEXTURE_BIND_SHADER_VIEW;
	td.format = yasp::TextureFormat::UNORM8_4;
	td.height = imh;
	td.mipLevels = 1;
	td.sampleCount = 1;
	td.sampleQuality = 0;
	td.usage = yasp::Usage::GPU_READ;
	td.width = imw;
	
	auto texture2d = pipe->CreateTexture2D(td, imdata);
	stbi_image_free(imdata);

	imdata = stbi_load("floor.png", &imw, &imh, &imc, 4);
	td.height = imh;
	td.width = imw;
	auto floortex = pipe->CreateTexture2D(td, imdata);
	stbi_image_free(imdata);
	yasp::Texture2DViewDesc tvd;
	tvd.format = yasp::TextureFormat::UNORM8_4;
	tvd.mipLevels = 1;
	tvd.mostDetailedMip = 0;

	auto srv = pipe->CreateTexture2DView(tvd, texture2d);
	auto srvFloor = pipe->CreateTexture2DView(tvd, floortex);

	

	yasp::SamplerDesc samd;
	samd.borderColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	samd.comparisonFunc = yasp::ComparisonFunc::ALWAYS;
	samd.filter = yasp::TextureFilter::ANISOTROPIC;
	samd.maxAnisotropy = 16;
	samd.maxLOD = (3.402823466e+38f);
	samd.minLOD = 0.0f;
	samd.mipLODBias = 0;
	samd.wrapModeU = yasp::TextureWrapping::REPEAT;
	samd.wrapModeV = yasp::TextureWrapping::REPEAT;
	samd.wrapModeW = yasp::TextureWrapping::REPEAT;
	
	auto sampler = pipe->CreateSampler(samd);

	pipe->SetShaderSamplers(yasp::ShaderType::PIXEL, &sampler, 0, 1);
	pshader["albedo"] = srv;
	pshader["samAnis"] = sampler;
	

	float speed = 3.0f;
	float rotSpeed = 2.25f;
	float lang = 0.0f;

	yasp::Timer timer;
	float rot = 0;
	float accTime = 0.0f;
	while (window.IsOpen())
	{
		window.PollEvents();
		float dt = timer.Tick();
		accTime += dt;
		auto mp = yasp::float2((float)yasp::Mouse::GetPos().x, (float)yasp::Mouse::GetPos().y);
		if (yasp::Keyboard::IsKeyDown(yasp::Keyboard::Key::A))
		{
			auto right = yasp::cross(up, forward);
			pos -= right * dt * speed;
		}
		if (yasp::Keyboard::IsKeyDown(yasp::Keyboard::Key::D))
		{
			auto right = yasp::cross(up, forward);
			pos += right * dt * speed;
		}
		if (yasp::Keyboard::IsKeyDown(yasp::Keyboard::Key::W))
		{
			pos += forward * dt * speed;
		}
		if (yasp::Keyboard::IsKeyDown(yasp::Keyboard::Key::S))
		{
			pos -= forward * dt * speed;
		}
		if (yasp::Keyboard::IsKeyDown(yasp::Keyboard::Key::SPACE))
		{
			pos += yasp::float3(0, 1, 0) * dt * speed;
		}
		if (yasp::Keyboard::WasKeyReleased(yasp::Keyboard::Key::ESCAPE))
		{
			window.Close();
		}
		if (yasp::Keyboard::WasKeyPressed(yasp::Keyboard::Key::G))
		{
			window.SetGrabCursor(true);
		}
		auto dm = yasp::Mouse::GetRelativeMovement();
		if (dm.x)
		{
			auto rot = yasp::mat4::RotationAxis(yasp::float3(0,1,0), -dm.x * dt * rotSpeed);
			forward = yasp::normalize((rot * yasp::float4(forward, 0.0f)).xyz);
			up = yasp::normalize((rot * yasp::float4(up, 0.0f)).xyz);
		}
		if (dm.y)
		{
			auto rot = yasp::mat4::RotationAxis(yasp::cross(up, forward), -dm.y * dt * rotSpeed);
			forward = yasp::normalize((rot * yasp::float4(forward, 0.0f)).xyz);
			up = yasp::normalize((rot * yasp::float4(up, 0.0f)).xyz);
		}

		lang += dt;
		
		pl.x = 6 * cos(lang);
		pl.z = 6 * sin(lang);
		pls.pointlights[0] = pl;

		pshader["ObjectBuffer"]["pointlight"] = pl;
		pshader["ObjectBuffer"].Update();
		pshader["PointLights"]["ALLOFIT"] = pls;
		pshader["PointLights"]["pointlights"][1] = PointLight({ 0.0f, 1.0f, 0.0f, 5.0f });
		pshader["PointLights"].Update();
		pshader["EyePos"]["eyepos"] = yasp::float4(pos.xyz, 1.0f);
		pshader["EyePos"].Update();
		pshader.Bind();

		renderContext.Clear();
		auto model = yasp::mat4::Identity();
		view = yasp::mat4::LookToLH(pos, forward, up);
		pipe->SetVertexBuffer(vbuffer, sizeof(Vertex), 0);
		pipe->SetShaderTextureViews(yasp::ShaderType::PIXEL, &srv, 0, 1);
		em.ForEach([&](yasp::Entity e, Pos& p, Velocity& v)
		{
			p += dt * v;
			if (accTime > 0.0f)
			{
				auto dir = yasp::normalize(pos - p);
				auto dist = yasp::length(pos - p);
				if(dist > 3.0f)
				{
					v.xyz = dir.xyz;
				}
				else
				{
					auto tangential = yasp::cross(dir, { 0, 1, 0});
					v.xyz = tangential.xyz * 5;
				}
			}
			model = yasp::mat4::Scale(0.2f, 0.2f, 0.2f) * yasp::mat4::Translation(p);
			auto newmat = ~(model * view * projection);

			vshader["WorldBuffer"]["gWorld"] = ~model;
			vshader["WorldBuffer"].Update();
			vshader["ObjectBuffer"]["gWVP"] = newmat;
			vshader["ObjectBuffer"].Update();
			renderContext.Draw(14, 0);
		});
		if (accTime > 0.0f)
		{
			accTime = 0.0f;
		}
		pipe->SetVertexBuffer(floorbuffer, sizeof(Vertex), 0);
		auto floorMVP =  view * projection;
		auto newmat = ~(floorMVP);
		auto ident = yasp::mat4::Identity();
		vshader["ObjectBuffer"]["gWVP"] = newmat;
		vshader["ObjectBuffer"].Update();
		vshader["WorldBuffer"]["gWorld"] = ident;
		vshader["WorldBuffer"].Update();
		pipe->SetShaderTextureViews(yasp::ShaderType::PIXEL, &srvFloor, 0, 1);

		//pshader["ObjectBuffer"] = lightBuffer;
		pshader.Bind();
		renderContext.Draw(4, 0);
		renderContext.Display();
	}
	return 0;
}