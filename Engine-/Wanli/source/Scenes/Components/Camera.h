#pragma once

#include "Core/BasicTypes.h"
#include "Core/Math3D.h"
#include "Scenes/IComponent.h"
#include "Scenes/Helpers/Frustum.h"
#include "Scenes/Helpers/Ray.h"
#include "Scenes/Helpers/Skybox.h"

namespace Wanli
{
    class Camera : public IComponent
    {
    public:
        enum class EBackgroundType
        {
            SolidColor,
            Skybox,
        };
        static constexpr glm::vec3 kOriginalTarget = { 0.f, 0.f, -1.f };

        Camera() = default;
        Camera(const glm::vec3& pos,
               const glm::vec3& target,
               float fov,
               float nearZ,
               float farZ,
               EBackgroundType type = EBackgroundType::SolidColor)
            : IComponent("Camera")
            , mPos(pos)
            , mRotation(Quaternion(kOriginalTarget, target))
            , mFieldOfView(fov)
            , mNearZ(nearZ)
            , mFarZ(farZ)
            , mBackgroundType(type)
        {}
        virtual ~Camera() {};

        virtual void Initialize() override final {}
        virtual void Update() override final {}

        inline float GetNearZ() const { return mNearZ; }
        inline float GetFarZ() const { return mFarZ; }
        inline float GetFieldOfView() const { return mFieldOfView; }
        inline const glm::vec3& GetPos() const { return mPos; }
        inline const glm::quat& GetRotation() const { return mRotation; }
        inline const glm::vec3& GetVelocity() const { return mVelocity; }
        inline const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
        inline const glm::mat4& GetProjMatrix() const { return mProjMatrix; }
        inline const Frustum& GetFrustum() const { return mViewFrustum; }
        inline const Ray& GetRay() const { return mViewRay; }
        inline const EBackgroundType& GetBackgroundType() const { return mBackgroundType; }
        inline const Color& GetBackgroundColor() const { return mBackgroundColor; }
        inline const Skybox& GetSkybox() const { return mSkybox; }

        inline void SetNearZ(float nearZ) { mNearZ = nearZ; }
        inline void SetFarZ(float farZ) { mFarZ = farZ; }
        inline void SetFieldOfView(float fov) { mFieldOfView = fov; }
        inline void SetPos(const glm::vec3& pos) { mPos = pos; }
        inline void Rotate(const glm::vec3& angles) { ApplyTo(mRotation, Quaternion(angles)); }
        inline void SetVelocity(const glm::vec3& velocity) { mVelocity = velocity; }
        inline void SetBackgroundColor(const Color& color) { mBackgroundColor = color; }
        inline void SetSkybox(const Skybox& skybox) { mSkybox = skybox; }

    protected:
        float mFieldOfView;
        float mNearZ;
        float mFarZ;

        glm::vec3 mPos;
        glm::quat mRotation;
        glm::vec3 mVelocity;

        glm::mat4 mViewMatrix;
        glm::mat4 mProjMatrix;

        Frustum mViewFrustum;
        Ray mViewRay;

        EBackgroundType mBackgroundType;
        Color mBackgroundColor;
        Skybox mSkybox;
    };
}