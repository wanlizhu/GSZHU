#include <iostream>
#include "Core/Engine.h"

using namespace Wanli;

int main(int argc, char** argv)
{
    WindowCreateInfo windowCreateInfo;
    windowCreateInfo.title = "Wanli Engine Editor";
    windowCreateInfo.size = { -1, -1 };
    windowCreateInfo.createMenu = true;
    windowCreateInfo.supportAPI = WindowCreateInfo::EGraphicsAPI::Vulkan;

    Engine::Get()->SetApp(std::move(MakeApplication(windowCreateInfo)));
    auto app = Engine::Get()->GetApp();

    Engine::Get()->GetConfig<InputConfig>()
        .SetApp(app);

    Engine::Get()->GetConfig<GraphicsConfig>()
        .SetDeviceType(ERHIDevice::Vulkan)
        .SetWindowHandle(app->GetWindowHandle())
        .AddEnabledLayers(app->GetRequiredVulkanLayers())
        .AddEnabledExtensions(app->GetRequiredVulkanExtensions())
        .SetBackgroundColor(Color_AutoCAD)
        .SetBackbufferCount(2);

    Engine::Get()->GetConfig<SceneConfig>()
        .SetWorldType(EWorldType::BVH)
        .LoadScene("");

    return Engine::Get()->Run();
}