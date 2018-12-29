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
	
	auto view = yasp::mat4::LookToLH({ 0.0f, 0.0f, -2.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
	auto projection = yasp::mat4::PerspectiveLH(yasp::PI<float> / 4, 640.0f / 480.0f, 0.1f, 10.0f);
	yasp::mat4 matrix = ~(view * projection);

	
	auto resID = renderContext.ResourceManager()->CreateBuffer(bd, &matrix);
	{
		auto resCopy = resID;
		auto resCopy2 = resID;
		auto resCopy3 = resID;
	}
	auto pipe = renderContext.ResourceManager();
	auto vshader = pipe->CreateVertexShader("SimpleVS.hlsl");
	auto pshader = pipe->CreatePixelShader("SimplePS.hlsl");

	pipe->SetVertexShader(vshader);
	pipe->SetPixelShader(pshader);

	struct Vertex
	{
		yasp::vec3<float> pos;
	};

	Vertex vertices[] = {
		yasp::vec3<float>(0.0f, 0.0f, 0.0f),
		yasp::vec3<float>(-0.5f, 0.0f, 0.0f),
		yasp::vec3<float>(0.0f, 0.5f, 0.0f)
	};

	bd.bind = yasp::BIND_VERTEX_BUFFER;
	bd.size = 3 * sizeof(Vertex);
	bd.usage = yasp::Usage::GPU_READ_WRITE;
	bd.byteStride = 0;
	
	auto vbuffer = pipe->CreateBuffer(bd, vertices);

	pipe->SetVertexBuffer(vbuffer, sizeof(Vertex), 0);
	pipe->SetVertexShaderBuffer(resID, 0);
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
		pipe->UpdateBuffer(resID, &newmat, sizeof(newmat));
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
		renderContext.Draw(3, 0);
		renderContext.Display();
	}
	return 0;
}