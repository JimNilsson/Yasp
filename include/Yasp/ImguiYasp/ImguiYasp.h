#ifndef IMGUI_IMPL_YASP_H
#define IMGUI_IMPL_YASP_H

#include "imgui.h"

namespace yasp
{
	class RenderContext;
	class Window;
}

IMGUI_IMPL_API bool ImGui_ImplYasp_Init(yasp::Window& window, yasp::RenderContext& renderContext);
IMGUI_IMPL_API void ImGui_ImplYasp_Shutdown();
IMGUI_IMPL_API void ImGui_ImplYasp_NewFrame();
IMGUI_IMPL_API void ImGui_ImplYasp_RenderDrawData(ImDrawData* drawData);

#endif
