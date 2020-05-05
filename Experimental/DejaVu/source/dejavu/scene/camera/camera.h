#pragma once

#include "utils/preprocess.h"
#include "utils/sample_patterns.h"

namespace glm
{
#include "scene/camera/camera_data.glsl.h"
}

namespace djv
{
    enum class CameraProperty
    {
        None = 0,
        Movement = 1,
        Exposure = 1 << 1,
        FocalDistance = 1 << 2,
        Jitter = 1 << 3,
        Frustum = 1 << 4,
        Aperture = 1 << 5,
    };
    ENUM_CLASS_OP(CameraProperty)

    struct BoundingBox;

    class DJV_API Camera
    {
    public:
        static constexpr float kDefaultFrameHeight = 24.0f;

        void beginFrame();
        void endFrame();
        bool isCulled(const BoundingBox& box) const;
        void togglePersistentProjMatrix(bool enable);
        void togglePersistentViewMatrix(bool enable);

        void setName(const std::string& name);
        void setAspectRatio(float ratio);
        void setFocalLength(float length);
        void setFocalDistance(float distance);
        void setFrameHeight(float height);
        void setApertureRadius(float radius);
        void setShutterSpeed(float speed);
        void setIsoSpeed(float speed);
        void setPosition(const glm::vec3& pos);
        void setUpVector(const glm::vec3& up);
        void setTarget(const glm::vec3& target);
        void setNearZ(float nearZ);
        void setFarZ(float farZ);
        void setDepthRange(float nearZ, float farZ);
        void setJitter(float jitterX, float jitterY);
        void setViewMatrix(const glm::mat4& viewMat);
        void setProjMatrix(const glm::mat4& projMat);
        void setSamplePattern(std::shared_ptr<SamplePatternInterface> pattern);

        const std::string& getName() const;
        float getAspectRatio() const;
        float getFocalLength() const;
        float getFocalDistance() const;
        float getFrameHeight() const;
        float getApertureRadius() const;
        float getShutterSpeed() const;
        float getIsoSpeed() const;
        float getNearZ() const;
        float getFarZ() const;
        float getJitterX() const;
        float getJitterY() const;
        const glm::vec3& getPosition() const;
        const glm::vec3& getUpVector() const;
        const glm::vec3& getTarget() const;
        const glm::mat4& getViewMatrix() const;
        const glm::mat4& getProjMatrix() const;
        const glm::mat4& getViewProjMatrix() const;
        const glm::mat4& getViewProjMatrixInv() const;
        std::shared_ptr<SamplePatternInterface> getSamplePattern() const;

    private:
        std::string mName;
        CameraProperty mChangedProperties = CameraProperty::None;

        std::optional<glm::mat4> mPersistentProjMat;
        std::optional<glm::mat4> mPersistentViewMat;
        
        glm::CameraData mCameraData;
        glm::CameraData mCameraDataPrev;

        struct {
            std::shared_ptr<SamplePatternInterface> generator;
            glm::vec2 scale;
        } mJitterPattern;

        struct {
            glm::vec3 pos;
            float     negW;
            glm::vec3 sign;
        } mFrustumPlanes[6];
    };
}