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
#include <Yasp/EntityComponent/Components/RadialLightEmitter.h>
#include <Yasp/ImguiYasp/ImguiYasp.h>

yasp::float3 randvec()
{
	float r = (rand() % 100) / 100.0f;
	float x = (rand() % 100) / 100.0f;
	float y = (rand() % 100) / 100.0f;
	float z = (rand() % 100) / 100.0f;
	return normalize(yasp::float3(x, y, z));
}

struct Vertex
{
	yasp::float3 pos;
	yasp::float3 nor;
	yasp::float2 tex;
};


struct Velocity : public yasp::float3
{
	using yasp::float3::float3;
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
	for (int i = 0; i < 50; i++)
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

	auto vshader = pipe->CreateVertexShader("SimpleVS.hlsl");
	yasp::gpu_components::VertexShader vs = yasp::GPUResourceID(vshader);
	for (auto& ent : entities)
	{
		em.Register(ent, vs);
	}
	
	auto pshader = pipe->CreatePixelShader("SimplePS.hlsl");
	auto rasterizer = pipe->CreateRasterizer({
		yasp::FillMode::SOLID,
		yasp::CullMode::BACK,
		yasp::WindingOrder::CLOCKWISE
	});

	vshader.Bind();
	pshader.Bind();
	pipe->SetRasterizer(rasterizer);

	
	Vertex positions[36] = {
		// Y+
		Vertex({ yasp::float3(0.5f, 0.5f, 0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, 0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, -0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(1.0f,1.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, 0.5f), yasp::float3(0.0f,1.0f,0.0f), yasp::float2(1.0f, 0.0f) }),

		// Z+
		Vertex({ yasp::float3(0.5f, 0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, 0.5f), yasp::float3(0.0f,0.0f,1.0f), yasp::float2(1.0f, 0.0f) }),

		// X+
		Vertex({ yasp::float3(0.5f, -0.5f, -0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, 0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, 0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, 0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(1.0f,0.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
		
		// Y-
		Vertex({ yasp::float3(0.5f, -0.5f, -0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, -0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, -0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, 0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(1.0f,1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(0.0f,-1.0f,0.0f), yasp::float2(0.0f, 1.0f) }),

		// Z-
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(0.5f, -0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(0.5f, 0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, -0.5f), yasp::float3(0.0f,0.0f,-1.0f), yasp::float2(1.0f, 0.0f) }),
		
		
		// X-
		Vertex({ yasp::float3(-0.5f, 0.5f, 0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f, -0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(1.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(0.0f, 1.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, -0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(0.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, -0.5f, 0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(1.0f, 0.0f) }),
		Vertex({ yasp::float3(-0.5f, 0.5f,-0.5f), yasp::float3(-1.0f,0.0f,0.0f), yasp::float2(1.0f, 1.0f) })		
	};
	yasp::BufferDesc bd;
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
		Vertex({ yasp::float3(10.0f, 0.0f, -10.0f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(30.0f, 0.0f) }),
		Vertex({ yasp::float3(-10.0f, 0.0f, 10.0f),yasp::float3(0.0f,1.0f,0.0f), yasp::float2(0.0f, 30.0f) }),
	};

	bd.size = 6 * sizeof(Vertex);
	auto floorbuffer = pipe->CreateBuffer(bd, floor);

	renderContext.SetTopology(yasp::Topology::TRIANGLE_LIST);
	renderContext.SetViewport({
		0.0f,
		0.0f,
		(float)window.GetWidth(),
		(float)window.GetHeight(),
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

	imdata = stbi_load("white.png", &imw, &imh, &imc, 4);
	td.height = imh;
	td.width = imw;
	auto whitetex = pipe->CreateTexture2D(td, imdata);
	stbi_image_free(imdata);

	yasp::Texture2DViewDesc tvd;
	tvd.format = yasp::TextureFormat::UNORM8_4;
	tvd.mipLevels = 1;
	tvd.mostDetailedMip = 0;

	auto srv = pipe->CreateTexture2DView(tvd, texture2d);
	auto srvFloor = pipe->CreateTexture2DView(tvd, floortex);
	auto srvWhite = pipe->CreateTexture2DView(tvd, whitetex);

	

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
	
	
	for (int i = 0; i < 6; i++)
	{
		auto ent = em.Create();
		yasp::RadialLightEmitter emitter;
		emitter.position = { (float)i, 0.25f, 0.0f };
		emitter.color = randvec();
		em.Register(ent, emitter);
	}

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
	bool lookMode = false;

	while (window.IsOpen())
	{
		window.PollEvents();
		ImGui_ImplYasp_NewFrame();
		ImGui::NewFrame();

		float dt = timer.Tick();
		accTime += dt;
		
		auto mp = yasp::float2((float)yasp::Mouse::GetPos().x, (float)yasp::Mouse::GetPos().y);
		if (lookMode)
		{
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
				lookMode = false;
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
				forward = yasp::normalize((rot * yasp::float4(0.0f, 0.0f, 1.0f, 0.0f)).xyz);
				up = yasp::normalize((rot * yasp::float4(0.0f, 1.0f, 0.0f, 0.0f)).xyz);
			}
		}
		else
		{
			if (yasp::Keyboard::WasKeyReleased(yasp::Keyboard::Key::ESCAPE))
			{
				lookMode = true;
			}
		}
	
		pshader["EyePos"]["eyepos"] = yasp::float4(pos.xyz, 1.0f);
		pshader["EyePos"].Update();

		ImGui::Begin("Lights");
		uint32_t plCount = 0;
		em.ForEach([&](yasp::Entity e, yasp::RadialLightEmitter& emitter) {
			std::string numStr = std::string(" ") + std::to_string(plCount);
			ImGui::SliderFloat3((std::string("Position") + numStr).c_str(), &emitter.position.v[0], -10.0f, 10.0f, "%.2f");
			ImGui::SliderFloat3((std::string("Color") + numStr).c_str(), &emitter.color.v[0], 0.0f, 1.0f, "%.3f");
			ImGui::SliderFloat((std::string("Falloff exp") + numStr).c_str(), &emitter.falloffExponential, 0.01f, 2.0f, "%.2f");
			ImGui::SliderFloat((std::string("Falloff lin") + numStr).c_str(), &emitter.falloffLinear, 0.01f, 2.0f, "%.2f");
			ImGui::SliderFloat3((std::string("Direction") + numStr).c_str(), &emitter.direction.v[0], 0.0f, 1.0f, "%.3f");
			emitter.direction = yasp::normalize(emitter.direction);
			ImGui::SliderFloat((std::string("Dir. bias") + numStr).c_str(), &emitter.directionBias, -3.0f, 0.0f, "%.2f");
			ImGui::Separator();
			
			pshader["PointLights"]["pointLights"][plCount] = emitter;
			++plCount;
		});
		pshader["PointLights"]["pointLightCount"] = yasp::vec4<uint32_t>(plCount, 0, 0, 0);
		pshader["PointLights"].Update();
		ImGui::End();

		pshader.Bind();

		renderContext.Clear();
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
		em.ForEach([&](yasp::Entity e, yasp::gpu_components::VertexShader& vertexShaderId)
		{
			auto vertexShader = pipe->GetShader(vertexShaderId);
			vertexShader.OnEachBuffer([&](yasp::GPUBuffer buffer)
			{
				buffer.OnEachElement([&](const std::string& identifier, yasp::AssignableMemory bufferSegment)
				{
					em.Request(identifier, e, bufferSegment);
				});
				buffer.Update();
			});
			renderContext.Draw(36, 0);
		});

		if (ImGui::Button("Exit Program"))
		{
			window.Close();
			break;
		}
		bool show = true;
		ImGui::ShowDemoWindow(&show);
			
		pipe->SetVertexBuffer(floorbuffer, sizeof(Vertex), 0);
		
		auto ident = yasp::mat4::Identity();
		auto floorWvp = cm.GetActiveViewProjectionMatrix();

		vshader["EntityBuffer"]["WorldViewProjectionMatrix"] = ~floorWvp;
		vshader["EntityBuffer"].Update();
		vshader["WorldBuffer"]["WorldMatrix"] = ident;
		vshader["WorldBuffer"]["RotationMatrix"] = ident;
		vshader["WorldBuffer"].Update();
		pipe->SetShaderTextureViews(yasp::ShaderType::PIXEL, &srvFloor, 0, 1);

		pshader.Bind();
		renderContext.Draw(6, 0);
		ImGui::Render();
		ImGui_ImplYasp_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();
		renderContext.Display();
	}
	ImGui_ImplYasp_Shutdown();
	ImGui::DestroyContext();
	return 0;
}