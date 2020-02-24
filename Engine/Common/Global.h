#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <stdint.h>

#include "Vector.h"
#include "FPS.h"
#include "IECSWorld.h"
#include "ECSWorld.h"
#include "Configuration.h"

namespace Engine
{
    enum ESystemType
    {
        eSystem_Event = 0,
        eSystem_Animation = 1,
        eSystem_Drawing = 2,
        eSystem_Scene = 3,
        eSystem_Input = 4,
        eSystem_Log = 5,
    };

    enum ERendererType
    {
        eRenderer_Start = 0,
        eRenderer_Forward = 0,
        eRenderer_Polyline,
        eRenderer_EllipseArc,
        eRenderer_End,
    };

    inline std::ostream& operator<< (std::ostream &out, EConfigurationDeviceType const& d)
    {
        if (d == eDevice_D3D11)
            return out << "D3D11";
        else if (d == eDevice_D3D12)
            return out << "D3D12";
        else if (d == eDevice_OGL)
            return out << "OGL";
		else if (d == eDevice_Vulkan)
			return out << "Vulkan";
        else
            return out << "Unknown";
    }

    class IApplication;
    class IEventSystem;
    class IAnimationSystem;
    class IDrawingSystem;
    class ISceneSystem;
    class IInputSystem;
    class ILogSystem;
    class IECSWorld;
    class IRenderer;

    class Global
    {
    public:
        Global();
        virtual ~Global();

        std::shared_ptr<IApplication> GetApplication();
        std::shared_ptr<IECSWorld> GetECSWorld();

        std::shared_ptr<IEventSystem> GetEventSystem();
        std::shared_ptr<IAnimationSystem> GetAnimationSystem();
        std::shared_ptr<IDrawingSystem> GetDrawingSystem();
        std::shared_ptr<ISceneSystem> GetSceneSystem();
        std::shared_ptr<IInputSystem> GetInputSystem();
        std::shared_ptr<ILogSystem> GetLogSystem();
        // Renderer
        std::shared_ptr<IRenderer> GetRenderer(ERendererType type);

        template<typename T>
        T& GetConfiguration()
        {
            return m_config.GetConfiguration<T>();
        }

        FPSCounter& GetFPSCounter();
        bool& IsInitialized();

        template<typename T>
        void RegisterApp()
        {
            m_pWorld = std::make_shared<ECSWorld>();
            auto app = std::make_shared<T>();
            auto result = std::dynamic_pointer_cast<IApplication>(app);
            m_pApp = result;
        }

        template<typename T>
        void RegisterRuntimeModule(ESystemType e)
        {
            auto it = m_pSystems.find(e);
            if (it != m_pSystems.end())
                return;
            auto system = std::make_shared<T>();
            auto result = std::dynamic_pointer_cast<IECSSystem>(system);

            m_pSystems[e] = result;
        }

        template<typename T>
        void RegisterRenderer(ERendererType e)
        {
            auto it = m_pRenderers.find(e);
            if (it != m_pRenderers.end())
                return;
            auto renderer = std::make_shared<T>();
            auto result = std::dynamic_pointer_cast<IRenderer>(renderer);

            m_pRenderers[e] = result;
        }

    private:
        std::shared_ptr<IECSSystem> GetRuntimeModule(ESystemType e);

    private:
        std::shared_ptr<IECSWorld> m_pWorld;

        std::shared_ptr<IApplication> m_pApp;
        std::unordered_map<ESystemType, std::shared_ptr<IECSSystem>> m_pSystems;
        std::unordered_map<ERendererType, std::shared_ptr<IRenderer>> m_pRenderers;

        Configuration m_config;
        FPSCounter m_fps;
        bool m_bInitialized = false;
    };

    class Setup
    {
    public:
        Setup();
        virtual ~Setup() {};
    };

    extern Global* gpGlobal;
}