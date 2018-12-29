#include <Yasp/Window/Window.h>
#include <Yasp/Rendering/RenderContext.h>
#include <Yasp/Rendering/Descriptions.h>
#include <Yasp/Window/Keyboard.h>
#include <Yasp/Window/Mouse.h>
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
	yasp::mat4 matrix = yasp::mat4::Identity();
	auto resID = renderContext.ResourceManager()->CreateBuffer(bd, &matrix);
	{
		auto resCopy = resID;
		auto resCopy2 = resID;
		auto resCopy3 = resID;
	}

	auto shader = renderContext.ResourceManager()->CreateVertexShader("SimpleVS.hlsl");
	

	while (window.IsOpen())
	{
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
		renderContext.Display();
	}
	return 0;
}