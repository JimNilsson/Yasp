#include <Yasp/Window/Window.h>
#include <Yasp/Rendering/RenderContext.h>
#include <Yasp/Rendering/Descriptions.h>
#include <Yasp/Window/Keyboard.h>
#include <Yasp/Window/Mouse.h>
#include <Yasp/Timer.h>
#include <iostream>
int main(int argc, char** argv)
{
	yasp::Window window(640, 480);
	yasp::RenderContext renderContext(window.GetWindowHandle());
	
	

	yasp::BufferDesc bd;
	bd.bind = yasp::BIND_SHADER_BUFFER;
	bd.byteStride = 0;
	bd.size = 16 * sizeof(float);
	bd.usage = yasp::Usage::GPU_READ_CPU_WRITE;
	
	auto view = yasp::mat4::LookToLH({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
	auto projection = yasp::mat4::PerspectiveLH(yasp::PI<float> / 4, 640.0f / 480.0f, 0.1f, 10.0f);
	yasp::mat4 matrix = ~(view * projection);

	
	auto wvpBuffer = renderContext.ResourceManager()->CreateBuffer(bd, &matrix);
	auto pipe = renderContext.ResourceManager();
	auto vshader = pipe->CreateVertexShader("SimpleVS.hlsl");
	auto pshader = pipe->CreatePixelShader("SimplePS.hlsl");
	auto rasterizer = pipe->CreateRasterizer({
		yasp::FillMode::SOLID,
		yasp::CullMode::NONE,
		yasp::WindingOrder::CLOCKWISE
	});

	
	pipe->SetVertexShader(vshader);
	pipe->SetPixelShader(pshader);
	pipe->SetRasterizer(rasterizer);

	yasp::float3 positions[14] = {
		yasp::float3(-1.0f, -1.0f, -1.0f),
		yasp::float3(1.0f, -1.0f, -1.0f),
		yasp::float3(1.0f, -1.0f,  1.0f),
		yasp::float3(1.0f,  1.0f, -1.0f),
		yasp::float3(1.0f,  1.0f,  1.0f),
		yasp::float3(-1.0f,  1.0f,  1.0f),
		yasp::float3(1.0f, -1.0f,  1.0f),
		yasp::float3(-1.0f, -1.0f,  1.0f),
		yasp::float3(-1.0f, -1.0f, -1.0f),
		yasp::float3(-1.0f,  1.0f,  1.0f),
		yasp::float3(-1.0f,  1.0f, -1.0f),
		yasp::float3(1.0f,  1.0f, -1.0f),
		yasp::float3(-1.0f, -1.0f, -1.0f),
		yasp::float3(1.0f, -1.0f, -1.0f)
	};

	bd.bind = yasp::BIND_VERTEX_BUFFER;
	bd.size = 14 * sizeof(yasp::float3);
	bd.usage = yasp::Usage::GPU_READ_WRITE;
	bd.byteStride = 0;
	
	auto vbuffer = pipe->CreateBuffer(bd, positions);

	pipe->SetVertexBuffer(vbuffer, sizeof(yasp::float3), 0);
	pipe->SetShaderBuffers(yasp::Shader::VERTEX, &wvpBuffer, 0, 1);
	renderContext.SetTopology(yasp::Topology::TRIANGLE_STRIP);
	renderContext.SetViewport({
		0.0f,
		0.0f,
		640.0f,
		480.0f,
		0.0f,
		1.0f
		});

	yasp::Timer timer;
	float rot = 0;
	while (window.IsOpen())
	{
		float dt = timer.Tick();
		rot += 2 * yasp::PI<float> * dt;
		auto rmat = yasp::mat4::RotationY(rot);
		auto newmat = ~(rmat * view * projection);
		pipe->UpdateBuffer(wvpBuffer, &newmat, sizeof(newmat));
		window.PollEvents();
		if (yasp::Keyboard::IsKeyDown(yasp::Keyboard::Key::A))
		{
			std::cout << "A is down\n";
		}
		if (yasp::Keyboard::WasKeyDoubleTapped(yasp::Keyboard::Key::B))
		{
			std::cout << "B was dtapped\n";
		}

		renderContext.Clear();
		renderContext.Draw(14, 0);
		renderContext.Display();
	}
	return 0;
}