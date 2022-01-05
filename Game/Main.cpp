#include <Yasp/Window/Window.h>
#include <Yasp/Rendering/RenderContext.h>
#include <Yasp/Rendering/Descriptions.h>
#include <Yasp/EntityComponent/Entity.h>
#include <Yasp/EntityComponent/EntityManager.h>
#include <Yasp/EntityComponent/TransformSystem.h>
#include <Yasp/EntityComponent/RenderSystem.h>
#include <Yasp/EntityComponent/CameraSystem.h>
#include <Yasp/Window/Keyboard.h>
#include <Yasp/Window/Mouse.h>
#include <Yasp/Timer.h>
#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <functional>
#include <Yasp/Rendering/AssignableMemory.h>
#include <unordered_map>
#include <algorithm>
#include <Yasp/EntityComponent/Components/IComponent.h>
#include <Yasp/EntityComponent/Components/Lens.h>
#include <Yasp/ImguiYasp/ImguiYasp.h>

yasp::float3 randvec()
{
	float r = (rand() % 100) / 100.0f;
	float x = (rand() % 100) / 100.0f;
	float y = (rand() % 100) / 100.0f;
	float z = (rand() % 100) / 100.0f;
	return normalize(yasp::float3(x, y, z));
}


struct Velocity : public yasp::IComponent, public yasp::float3
{
	using yasp::float3::float3;
	static constexpr const char* Name() { return "Velocity"; }
};

struct PointLight : public yasp::float4
{
	using yasp::float4::float4;
};



#define STB_IMAGE_IMPLEMENTATION
#include <Yasp/stb_image.h>
int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	yasp::Window window(1280, 720);
	window.SetGrabCursor(true);
	yasp::RenderContext renderContext(window.GetWindowHandle());
	auto pipe = renderContext.ResourceManager();
	yasp::EntityManager em;
	yasp::TransformSystem tm(em);
	yasp::RenderSystem rm(em, renderContext);
	yasp::CameraSystem cm(em);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplYasp_Init(window, renderContext);


	yasp::Entity e = {};
	std::vector<yasp::Entity> entities;
	for (int i = 0; i < 500; i++)
	{
		auto ent = em.Create();
		e = ent;
		entities.push_back(ent);
		auto p = randvec().xyz * 2.0f;
		auto dir = randvec().xyz * 1.0f;
		tm.Register(ent, { p.x,p.y,p.z, 1.0f }, { 1.0f,0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f,0.0f });
		em.Register(ent, Velocity(dir.x, dir.y, dir.z));
	}

	auto cameraEntity = em.Create();
	auto camera2 = em.Create();
	em.Register(cameraEntity, yasp::Lens(), yasp::Position(0.0f, 0.0f, 0.0f, 1.0f), yasp::Rotation(1.0f, 0.0f, 0.0f, 0.0f));
	em.Register(camera2, yasp::Lens(), yasp::Position(0.0f, 0.0f, 0.0f, 1.0f), yasp::Rotation(1.0f, 0.0f, 0.0f, 0.0f));
	cm.SetActiveCamera(cameraEntity);

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
	
	
	auto vshader = pipe->CreateVertexShader("SimpleVS.hlsl");
	yasp::gpu_components::VertexShader vs = yasp::GPUResourceID(vshader);
	for (auto& ent : entities)
	{
		em.Register(ent, vs);
	}
	
	auto pshader = pipe->CreatePixelShader("SimplePS.hlsl");
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

	
	Vertex positions[36] = {
		// Y+
		Vertex({ yasp::float3(0.5f, 0.5f, 0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, 0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, -0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(1.0f,1.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, 0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(1.0f, 0.0f) }),

		// Z+
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(1.0f, 0.0f) }),

		// X+
		Vertex({ yasp::float3(0.5f, -0.5f, -0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, 0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, 0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, 0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
		
		// Y-
		Vertex({ yasp::float3(-0.5f, -0.5f, -0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, -0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, 0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(1.0f,1.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, -0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(1.0f, 0.0f) }),

		// Z-
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(1.0f, 0.0f) }),

		// X-
		Vertex({ yasp::float3(-0.5f, 0.5f, 0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, -0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, -0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f,-0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
	};

	bd.bind = yasp::BIND_VERTEX_BUFFER;
	bd.size = 36 * sizeof(Vertex);
	bd.usage = yasp::Usage::GPU_READ_WRITE;
	bd.byteStride = 0;
	
	auto vbuffer = pipe->CreateBuffer(bd, positions);

	Vertex floor[6] = {
		Vertex({ yasp::float3(-10.0f, 0.0f, -10.0f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-10.0f, 0.0f, 10.0f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(30.0f, 0.0f) }),
		Vertex({ yasp::float3(10.0f, 0.0f, -10.0f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 30.0f) }),
		Vertex({ yasp::float3(10.0f, 0.0f, 10.0f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(30.0f, 30.0f) }),
		Vertex({ yasp::float3(-10.0f, 0.0f, 10.0f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(30.0f, 0.0f) }),
		Vertex({ yasp::float3(10.0f, 0.0f, -10.0f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 30.0f) }),
	};

	bd.size = 6 * sizeof(Vertex);
	auto floorbuffer = pipe->CreateBuffer(bd, floor);

	pipe->SetVertexBuffer(vbuffer, sizeof(Vertex), 0);

	renderContext.SetTopology(yasp::Topology::TRIANGLE_LIST);
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
	float rotSpeed = 1.0f;
	float lang = 0.0f;

	yasp::Timer timer;
	float rot = 0;
	float accTime = 0.0f;
	yasp::float3 pos = { 0, 0, 0 };
	yasp::float3 forward = { 0,0,1 };
	yasp::float3 up = { 0,1,0 };
	yasp::quaternion cameraRotation = { 1.0f, 0.0f, 0.0f, 0.0f };
	while (window.IsOpen())
	{
		window.PollEvents();
		ImGui_ImplYasp_NewFrame();
		ImGui::NewFrame();

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
			break;
		}
		if (yasp::Keyboard::WasKeyPressed(yasp::Keyboard::Key::G))
		{
			window.SetGrabCursor(true);
		}
		auto dm = yasp::Mouse::GetRelativeMovement();
		if (dm.x)
		{
			float rotAngle = -dm.x*dt*rotSpeed;
			cameraRotation *= yasp::quaternion(std::cosf(rotAngle), 0, std::sinf(rotAngle), 0);
			auto rot = yasp::mat4::RotationQuaternion(cameraRotation);
			forward = yasp::normalize((rot * yasp::float4(0.0f, 0.0f, 1.0f, 0.0f)).xyz);
			up = yasp::normalize((rot * yasp::float4(0.0f, 1.0f, 0.0f, 0.0f)).xyz);
		}
		if (dm.y)
		{
			auto right = cross(up, forward);
			float rotAngle = -dm.y*dt*rotSpeed;
			right *= std::sinf(rotAngle);
			cameraRotation *= yasp::quaternion(std::cosf(rotAngle), right.x, right.y, right.z);
			auto rot = yasp::mat4::RotationQuaternion(cameraRotation);
			forward = yasp::normalize((rot * yasp::float4(0.0f,0.0f,1.0f, 0.0f)).xyz);
			up = yasp::normalize((rot * yasp::float4(0.0f,1.0f,0.0f, 0.0f)).xyz);
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
		em.ForEach([&](yasp::Entity e, yasp::Position& p, Velocity& v)
		{
			p.xyz += dt * v;

			if (p.x >= 10.0f || p.x <= -10.0f)
			{
				v.x = -v.x;
			}
			if (p.y >= 4.0f || p.y <= 0.0f)
			{
				v.y = -v.y;
			}
			if (p.z >= 10.0f || p.z <= -10.0f)
			{
				v.z = -v.z;
			}
		});

		em.Update(cameraEntity, yasp::Position(pos.xyz, 1.0f), yasp::Rotation(cameraRotation.x, cameraRotation.y, cameraRotation.z, cameraRotation.w));
		em.FireUpdateCallbacks();
		cm.SetActiveCamera(cameraEntity);
		//em.ForEach([&](yasp::Entity e, yasp::gpu_components::VertexShader& vertexShaderId)
		//{
		//	auto vertexShader = pipe->GetShader(vertexShaderId);

		//	vertexShader.OnEachBuffer([&](yasp::GPUBuffer buffer)
		//	{
		//		buffer.OnEachElement([&](const std::string& identifier, yasp::AssignableMemory bufferSegment)
		//		{
		//			em.Request(identifier, e, bufferSegment);
		//		});
		//		buffer.Update();
		//	});

		//	renderContext.Draw(36, 0);
		//	
		//});
		ImGui::Text("Hello Imgui Rendered with Yasp!");
		ImGui::Text("The text is a bit weird though, but it works!");
		//pipe->SetVertexBuffer(floorbuffer, sizeof(Vertex), 0);
		//auto floorMVP =  view * projection;
		//auto newmat = ~(floorMVP);
		//auto ident = yasp::mat4::Identity();
		//vshader["EntityBuffer"]["WorldViewProjectionMatrix"] = newmat;
		//vshader["EntityBuffer"].Update();
		//vshader["WorldBuffer"]["WorldMatrix"] = ident;
		//vshader["WorldBuffer"]["RotationMatrix"] = ident;
		//vshader["WorldBuffer"].Update();
		//pipe->SetShaderTextureViews(yasp::ShaderType::PIXEL, &srvFloor, 0, 1);

		//pshader.Bind();
	//	renderContext.Draw(6, 0);
		ImGui::Render();
		ImGui_ImplYasp_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();
		renderContext.Display();
	}
	ImGui_ImplYasp_Shutdown();
	ImGui::DestroyContext();
	return 0;
}