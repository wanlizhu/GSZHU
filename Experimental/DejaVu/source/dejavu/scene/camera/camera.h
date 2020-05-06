#pragma once

#include <glm/glm.hpp>
#include "utils/preprocess.h"
#include "utils/sample_patterns.h"

namespace glm
{
#include "scene/camera/camera_data.glsl.h"
}

namespace djv
{
    struct BoundingBox;

    class DJV_API Camera
    {
    public:
        /* Default dimensions of full frame cameras and 35mm film */
        static constexpr float kDefaultFrameHeight = 24.0f;

        void beginFrame();
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
        void setSamplePattern(std::shared_ptr<SamplePatternInterface> pattern, glm::vec2 scale = { 1.0f, 1.0f });

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
        void calculateCameraParameters() const;
        void setJitterInternal(float jitterX, float jitterY);

    private:
        std::string mName;
        mutable bool mDirty = true;

        bool mPersistentProjMatEnabled = false;
        bool mPersistentViewMatEnabled = false;
        std::optional<glm::mat4> mPersistentProjMat;
        std::optional<glm::mat4> mPersistentViewMat;
        
        mutable glm::CameraData mCameraData;
        glm::CameraData mCameraDataPrev;

        struct {
            std::shared_ptr<SamplePatternInterface> generator;
            glm::vec2 scale;
        } mJitterPattern;

        struct {
            glm::vec3 xyz;
            float     negW;
            glm::vec3 sign;
        } mutable mFrustumPlanes[6];
    };
}