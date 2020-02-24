#include <random>

#include "WindowsApplication.h"
#include "EventSystem.h"
#include "AnimationSystem.h"
#include "DrawingSystem.h"
#include "SceneSystem.h"
#include "InputSystem.h"
#include "LogSystem.h"

#include "AnimationComponent.h"
#include "CameraComponent.h"
#include "MeshFilterComponent.h"
#include "MeshRendererComponent.h"
#include "TransformComponent.h"
#include "Component\PolylineRendererComponent.h"
#include "Component\EllipseArcRendererComponent.h"

#include "CubeMesh.h"
#include "PlaneMesh.h"
#include "GLTF2Mesh.h"

#include "ForwardRenderer.h"
#include "PolylineRenderer.h"
#include "EllipseArcRenderer.h"

#include "GobFileLoader/GobFileLoader.h"

using namespace Engine;
using namespace Platform;
using namespace GobFileLoader;
IComponent::CompTableType IComponent::m_compTable;

class GameSetup : public Setup
{
protected:
    void AddPolylines(const CGobFileLoader& loader, PolylineRendererComponent& polylineRendererComp)
    {
        for (uint32_t i = 0; i < loader.lines.size(); ++i) {
            GobFileLoader::line* l = (GobFileLoader::line*)(loader.lines[i]);

            PolylineRendererComponent::LineSegmentGeom geom;
            geom.startPoint = float2(l->p1.x, l->p1.y);
            geom.endPoint   = float2(l->p2.x, l->p2.y);
            geom.color      = l->color;
            geom.drawZ      = 1.0f;
            polylineRendererComp.SetGeometry(geom);
        }
        gpGlobal->GetECSWorld()->CreateEntity<PolylineRendererComponent>(polylineRendererComp);
    }

    void AddEllipseArcs(const CGobFileLoader& loader, EllipseArcRendererComponent& ellipseArcRendererComp)
    {
        for (uint32_t i = 0; i < loader.circles.size(); ++i) {
            GobFileLoader::ellipse* e = (GobFileLoader::ellipse*)(loader.circles[i]);

            EllipseArcRendererComponent::EllipseArcGeom geom;
            geom.center     = float2(e->center.x, e->center.y);
            geom.radius     = float2(e->radius.x, e->radius.y);
            geom.range      = float2(e->range.x, e->range.y);
            geom.rotate     = e->rotate;
            geom.color      = e->color;
            geom.drawZ      = e->drawZ;
            ellipseArcRendererComp.SetGeometry(geom);
        }

        for (uint32_t i = 0; i < loader.ellipses.size(); ++i) {
            GobFileLoader::ellipse* e = (GobFileLoader::ellipse*)(loader.ellipses[i]);

            EllipseArcRendererComponent::EllipseArcGeom geom;
            geom.center     = float2(e->center.x, e->center.y);
            geom.radius     = float2(e->radius.x, e->radius.y);
            geom.range      = float2(e->range.x, e->range.y);
            geom.rotate     = e->rotate;
            geom.color      = e->color;
            geom.drawZ      = e->drawZ;
            ellipseArcRendererComp.SetGeometry(geom);
        }

        gpGlobal->GetECSWorld()->CreateEntity<EllipseArcRendererComponent>(ellipseArcRendererComp);
    }

public:
    GameSetup()
    {
        gpGlobal->GetConfiguration<AppConfiguration>().SetAppName("Game Test");
        gpGlobal->GetConfiguration<AppConfiguration>().SetWidth(1920);
        gpGlobal->GetConfiguration<AppConfiguration>().SetHeight(1080);
        gpGlobal->GetConfiguration<GraphicsConfiguration>().SetDeviceType(EConfigurationDeviceType::eDevice_Vulkan);
        gpGlobal->GetConfiguration<GraphicsConfiguration>().SetMSAA(eMSAA_Disable);

        gpGlobal->RegisterApp<WindowsApplication>();

        gpGlobal->RegisterRuntimeModule<InputSystem>(eSystem_Input);
        gpGlobal->RegisterRuntimeModule<EventSystem>(eSystem_Event);
        gpGlobal->RegisterRuntimeModule<SceneSystem>(eSystem_Scene);
        gpGlobal->RegisterRuntimeModule<AnimationSystem>(eSystem_Animation);
        gpGlobal->RegisterRuntimeModule<DrawingSystem>(eSystem_Drawing);
        gpGlobal->RegisterRuntimeModule<LogSystem>(eSystem_Log);

        gpGlobal->RegisterRenderer<ForwardRenderer>(eRenderer_Forward);
        //gpGlobal->RegisterRenderer<PolylineRenderer>(eRenderer_Polyline);
        //gpGlobal->RegisterRenderer<EllipseArcRenderer>(eRenderer_EllipseArc);

        auto& pWorld = gpGlobal->GetECSWorld();

        // ECSSystem
        pWorld->AddECSSystem(gpGlobal->GetInputSystem());
        pWorld->AddECSSystem(gpGlobal->GetEventSystem());
        pWorld->AddECSSystem(gpGlobal->GetAnimationSystem());
        pWorld->AddECSSystem(gpGlobal->GetDrawingSystem());
        pWorld->AddECSSystem(gpGlobal->GetLogSystem());
        
        // Plane
        TransformComponent planeTransformComp;
        MeshFilterComponent planeMeshFilterComp;
        MeshRendererComponent planeMeshRendererComp;
        planeTransformComp.SetPosition(float3(0.0f, -0.5f, 0.0f));
        auto pPlaneMesh = std::make_shared<PlaneMesh>();
        planeMeshFilterComp.SetMesh(pPlaneMesh);
        auto pPlane = pWorld->CreateEntity<TransformComponent, MeshFilterComponent, MeshRendererComponent>(planeTransformComp, planeMeshFilterComp, planeMeshRendererComp);

        // Entity 1
        TransformComponent cubeTransformComp1;
        MeshFilterComponent cubeMeshFilterComp1;
        MeshRendererComponent cubeMeshRendererComp1;
        AnimationComponent cubeAnimationComp1;
        //auto pMesh = std::make_shared<CubeMesh>();
        auto pMesh = std::make_shared<GLTF2Mesh>("Asset/Scene/Test/DamagedHelmet.gltf");
        cubeMeshFilterComp1.SetMesh(pMesh);
        auto pCube1 = pWorld->CreateEntity<TransformComponent, MeshFilterComponent, MeshRendererComponent, AnimationComponent>(cubeTransformComp1, cubeMeshFilterComp1, cubeMeshRendererComp1, cubeAnimationComp1);
        AnimationComponent::AnimationFunc func = [pCube1](float elapsedTime) -> void
        {
            float second = elapsedTime / 1000;

            auto pTrans = pCube1->GetComponent<TransformComponent>();
            auto rotate = pTrans->GetRotate();
            rotate.y += second * 1.f;
            pTrans->SetRotate(rotate);
        };
        pCube1->GetComponent<AnimationComponent>()->SetAnimationFunc(func);

        // Entity 2
        TransformComponent cubeTransformComp2;
        MeshFilterComponent cubeMeshFilterComp2;
        MeshRendererComponent cubeMeshRendererComp2;
        AnimationComponent cubeAnimationComp;
        cubeTransformComp2.SetPosition(float3(0.0f, 0.5f, 1.0f));
        cubeTransformComp2.SetRotate(float3(0.0f, -1.0f, 0.0f));
        cubeTransformComp2.SetScale(float3(2.0f, 2.0f, 2.0f));
        auto pCubeMesh2 = std::make_shared<CubeMesh>();
        cubeMeshFilterComp2.SetMesh(pCubeMesh2);
        auto pCube2 = pWorld->CreateEntity<TransformComponent, MeshFilterComponent, MeshRendererComponent>(cubeTransformComp2, cubeMeshFilterComp2, cubeMeshRendererComp2);
        
        // Camera
        TransformComponent cameraPosComp;
        CameraComponent cameraComp;
        cameraPosComp.SetPosition(Vec3<float>(0.0f, 2.0f, -5.0f));
        auto camera = pWorld->CreateEntity<TransformComponent, CameraComponent>(cameraPosComp, cameraComp);

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distWidth(0, gpGlobal->GetConfiguration<AppConfiguration>().GetWidth());
        std::uniform_int_distribution<std::mt19937::result_type> distHeight(0, gpGlobal->GetConfiguration<AppConfiguration>().GetHeight());
        std::uniform_int_distribution<std::mt19937::result_type> distColor(0, 255);

        // Primitives
        PolylineRendererComponent polylineRendererComp;
        EllipseArcRendererComponent ellipseArcRendererComp;

        CGobFileLoader loader("Asset/Scene/Geometry/HT-01.gob");
        AddPolylines(loader, polylineRendererComp);
        AddEllipseArcs(loader, ellipseArcRendererComp);
    }

private:
    uint32_t pack_color(uint4 color)
    {
        return (color.z & 0x000000ff) |
        ((color.y & 0x000000ff) << 8) |
        ((color.x & 0x000000ff) << 16) |
        ((color.w & 0x000000ff) << 24);
    }
};

static GameSetup setup;