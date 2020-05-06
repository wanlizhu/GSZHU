#include "scene/camera/camera.h"
#include "utils/bounding_box.h"
#include "utils/math3d.h"
#include <glm/gtc/type_ptr.hpp>
#include <glog/logging.h>

static_assert(sizeof(glm::CameraData) % (sizeof(glm::vec4)) == 0, "CameraData size should be a multiple of 16");

namespace djv
{
    void Camera::beginFrame()
    {
        static bool _firstFrame = true;

        if (mJitterPattern.generator)
        {
            glm::vec2 jitter = mJitterPattern.generator->next();
            jitter *= mJitterPattern.scale;
            setJitterInternal(jitter.x, jitter.y);
        }

        calculateCameraParameters();

        if (_firstFrame) {
            mCameraDataPrev = mCameraData;
            _firstFrame = false;
        }
    }

    bool Camera::isCulled(const BoundingBox& box) const
    {
        calculateCameraParameters();

        bool isInside = true;

        // AABB vs. frustum test
        // See method 4b: https://fgiesen.wordpress.com/2010/10/17/view-frustum-culling/
        for (int plane = 0; plane < 6; plane++)
        {
            glm::vec3 signedExtent = box.extent * mFrustumPlanes[plane].sign;
            float dr = glm::dot(box.center + signedExtent, mFrustumPlanes[plane].xyz);
            isInside = isInside && (dr > mFrustumPlanes[plane].negW);
        }

        return !isInside;
    }

    void Camera::togglePersistentProjMatrix(bool enable)
    {
        mPersistentProjMatEnabled = enable;
    }

    void Camera::togglePersistentViewMatrix(bool enable)
    {
        mPersistentViewMatEnabled = enable;
    }

    void Camera::setName(const std::string& name)
    {
        mName = name;
    }

    void Camera::setAspectRatio(float ratio)
    {
        mCameraData.aspectRatio = ratio;
        mDirty = true;
    }

    void Camera::setFocalLength(float length)
    {
        mCameraData.focalLength = length;
        mDirty = true;
    }

    void Camera::setFocalDistance(float distance)
    {
        mCameraData.focalDistance = distance;
        mDirty = true;
    }

    void Camera::setFrameHeight(float height)
    {
        mCameraData.frameHeight = height;
        mDirty = true;
    }

    void Camera::setApertureRadius(float radius)
    {
        mCameraData.apertureRadius = radius;
        mDirty = true;
    }

    void Camera::setShutterSpeed(float speed)
    {
        mCameraData.shutterSpeed = speed;
        mDirty = true;
    }

    void Camera::setIsoSpeed(float speed)
    {
        mCameraData.isoSpeed = speed;
        mDirty = true;
    }

    void Camera::setPosition(const glm::vec3& pos)
    {
        mCameraData.posWorld = pos;
        mDirty = true;
    }

    void Camera::setUpVector(const glm::vec3& up)
    {
        mCameraData.up = up;
        mDirty = true;
    }

    void Camera::setTarget(const glm::vec3& target)
    {
        mCameraData.target = target;
        mDirty = true;
    }

    void Camera::setNearZ(float nearZ)
    {
        mCameraData.nearZ = nearZ;
        mDirty = true;
    }

    void Camera::setFarZ(float farZ)
    {
        mCameraData.farZ = farZ;
        mDirty = true;
    }

    void Camera::setDepthRange(float nearZ, float farZ)
    {
        mCameraData.nearZ = nearZ;
        mCameraData.farZ = farZ;
        mDirty = true;
    }

    void Camera::setJitter(float jitterX, float jitterY)
    {
        if (mJitterPattern.generator)
        {
            LOG(WARNING) << "Camera::setJitter() called when a pattern-generator object was attached to the camera. Detaching the pattern-generator";
            mJitterPattern.generator = nullptr;
        }

        setJitterInternal(jitterX, jitterY);
    }

    void Camera::setJitterInternal(float jitterX, float jitterY)
    {
        mCameraData.jitterX = jitterX;
        mCameraData.jitterY = jitterY;
        mDirty = true;
    }

    void Camera::setViewMatrix(const glm::mat4& viewMat)
    {
        mDirty = true;
        mPersistentViewMat = viewMat;
        togglePersistentViewMatrix(true);
    }

    void Camera::setProjMatrix(const glm::mat4& projMat)
    {
        mDirty = true;
        mPersistentProjMat = projMat;
        togglePersistentProjMatrix(true);
    }

    void Camera::setSamplePattern(std::shared_ptr<SamplePatternInterface> pattern, glm::vec2 scale)
    {
        mJitterPattern.generator = pattern;
        mJitterPattern.scale = scale;
        
        if (!mJitterPattern.generator) {
            setJitter(0.0f, 0.0f);
        }
    }

    const std::string& Camera::getName() const
    {
        return mName;
    }

    float Camera::getAspectRatio() const
    {
        return mCameraData.aspectRatio;
    }

    float Camera::getFocalLength() const
    {
        return mCameraData.focalLength;
    }

    float Camera::getFocalDistance() const
    {
        return mCameraData.focalDistance;
    }

    float Camera::getFrameHeight() const
    {
        return mCameraData.frameHeight;
    }

    float Camera::getApertureRadius() const
    {
        return mCameraData.apertureRadius;
    }

    float Camera::getShutterSpeed() const
    {
        return mCameraData.shutterSpeed;
    }

    float Camera::getIsoSpeed() const
    {
        return mCameraData.isoSpeed;
    }

    float Camera::getNearZ() const
    {
        return mCameraData.nearZ;
    }

    float Camera::getFarZ() const
    {
        return mCameraData.farZ;
    }

    float Camera::getJitterX() const
    {
        return mCameraData.jitterX;
    }

    float Camera::getJitterY() const
    {
        return mCameraData.jitterY;
    }

    const glm::vec3& Camera::getPosition() const
    {
        return mCameraData.posWorld;
    }

    const glm::vec3& Camera::getUpVector() const
    {
        return mCameraData.up;
    }

    const glm::vec3& Camera::getTarget() const
    {
        return mCameraData.target;
    }

    const glm::mat4& Camera::getViewMatrix() const
    {
        calculateCameraParameters();
        return mCameraData.viewMat;
    }

    const glm::mat4& Camera::getProjMatrix() const
    {
        calculateCameraParameters();
        return mCameraData.projMat;
    }

    const glm::mat4& Camera::getViewProjMatrix() const
    {
        calculateCameraParameters();
        return mCameraData.viewProjMat;
    }

    const glm::mat4& Camera::getViewProjMatrixInv() const
    {
        calculateCameraParameters();
        return mCameraData.viewProjMatInv;
    }

    std::shared_ptr<SamplePatternInterface> Camera::getSamplePattern() const
    {
        return mJitterPattern.generator;
    }

    void Camera::calculateCameraParameters() const
    {
        if (!mDirty) 
        {
            return;
        }

        const float yfov = mCameraData.focalLength == 0.0f ? 0.0f : focal_length_to_yfov(mCameraData.focalLength, mCameraData.frameHeight);
        
        if (mPersistentViewMatEnabled) 
        {
            mCameraData.viewMat = mPersistentProjMat.value();
        }
        else
        {
            mCameraData.viewMat = glm::lookAt(mCameraData.posWorld, mCameraData.target, mCameraData.up);
        }
        
        if (mPersistentProjMatEnabled)
        {
            mCameraData.projMat = mPersistentProjMat.value();
        } 
        else 
        {
            if (yfov == 0.0f)
            {
                const float halfLookAt = glm::length(mCameraData.posWorld - mCameraData.target) * 0.5f;
                mCameraData.projMat = glm::ortho(-halfLookAt, halfLookAt, -halfLookAt, halfLookAt, mCameraData.nearZ, mCameraData.farZ);
            } 
            else 
            {
                mCameraData.projMat = glm::perspective(yfov, mCameraData.aspectRatio, mCameraData.nearZ, mCameraData.farZ);
            }
        }

        /*
         * Build jitter matrix
         * (jitterX and jitterY are expressed as subpixel quantities divided by the screen resolution
         * for instance to apply an offset of half pixel along the X axis we set jitterX = 0.5f / Width) 
         */
        glm::mat4 jitterMat(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            2.0f * mCameraData.jitterX, 2.0f * mCameraData.jitterY, 0.0f, 1.0f
        );
        
        mCameraData.viewProjMatNoJitter = mCameraData.projMat * mCameraData.viewMat;
        mCameraData.projMat = jitterMat * mCameraData.projMat;

        mCameraData.viewProjMat = mCameraData.projMat * mCameraData.viewMat;
        mCameraData.viewProjMatInv = glm::inverse(mCameraData.viewProjMat);

        /*
         * Extract camera space frustum planes from the VP matrix
         * See: https://fgiesen.wordpress.com/2012/08/31/frustum-planes-from-the-projection-matrix/ 
         */
        glm::mat4 tmpMat = glm::transpose(mCameraData.viewProjMat);
        for (int i = 0; i < 6; i++)
        {
            glm::vec4 plane = (i & 1) ? tmpMat[i >> 1] : -tmpMat[i >> 1];
            if (i != 5) // Z range is [0, w]. For the 0 <= z plane we don't need to add w
            {
                plane += tmpMat[3];
            }

            mFrustumPlanes[i].xyz  = glm::vec3(plane);
            mFrustumPlanes[i].sign = glm::sign(mFrustumPlanes[i].xyz);
            mFrustumPlanes[i].negW = -plane.w;
        }

        // Ray tracing related vectors
        const float uLen = mCameraData.focalDistance * tanf(yfov * 0.5f) * mCameraData.aspectRatio;
        const float vLen = mCameraData.focalDistance * tanf(yfov * 0.5f);
        mCameraData.cameraW = glm::normalize(mCameraData.target - mCameraData.posWorld) * mCameraData.focalDistance;
        mCameraData.cameraU = glm::normalize(glm::cross(mCameraData.cameraW, mCameraData.up));
        mCameraData.cameraV = glm::normalize(glm::cross(mCameraData.cameraU, mCameraData.cameraW));
        mCameraData.cameraU *= uLen;
        mCameraData.cameraV *= vLen;
    }
}