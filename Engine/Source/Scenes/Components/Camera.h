#pragma once

#include "Core/Common.h"
#include "Scenes/IComponent.h"
#include "Utils/Frustum.h"
#include "Utils/Ray.h"
#include "Utils/Skybox.h"

namespace Wanli
{
    class WANLI_API Camera : public IComponent
    {
    public:
        enum class EBackgroundType
        {
            SolidColor,
            Skybox,
        };
        static constexpr glm::vec3 kOriginalTarget = { 0.f, 0.f, -1.f };

        Camera() = default;
        Camera(const glm::vec3& pos, const glm::vec3& target);
        virtual ~Camera();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void Destroy() override;

        inline float GetNearZ() const { return mNearZ; }
        inline float GetFarZ() const { return mFarZ; }
        inline float GetFieldOfView() const { return mFieldOfView; }
        inline glm::vec3 const& GetPos() const { return mPos; }
        inline glm::quat const& GetRotation() const { return mRotation; }
        inline glm::vec3 const& GetVelocity() const { return mVelocity; }
        inline glm::mat4 const& GetViewMatrix() const { return mViewMatrix; }
        inline glm::mat4 const& GetProjMatrix() const { return mProjMatrix; }
        inline Frustum const& GetFrustum() const { return mViewFrustum; }
        inline Ray const& GetRay() const { return mViewRay; }
        inline EBackgroundType const& GetBackgroundType() const { return mBackgroundType; }
        inline Color const& GetBackgroundColor() const { return mBackgroundColor; }
        inline Skybox const& GetSkybox() const { return mSkybox; }

        inline void SetNearZ(float nearZ) { mNearZ = nearZ; }
        inline void SetFarZ(float farZ) { mFarZ = farZ; }
        inline void SetFieldOfView(float fov) { mFieldOfView = fov; }
        inline void SetPos(const glm::vec3& pos) { mPos = pos; }
        inline void Rotate(float angle, const glm::vec3& axis) { mRotation = glm::rotate(mRotation, angle, axis); }
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