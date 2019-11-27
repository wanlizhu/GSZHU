#pragma once

#include "Surface.h"
#include "Texture.h"

namespace Wanlix
{
    class OffscreenSurface : public Surface
    {
    public:
        using Ptr = std::shared_ptr<OffscreenSurface>;
        using ConstPtr = std::shared_ptr<const OffscreenSurface>;

        static Ptr Create(int width, int height);
        virtual ~OffscreenSurface();

        virtual SurfaceType GetSurfaceType() const override;
        virtual void* GetNativeHandle() const override;
        virtual Extent GetContentSize() const override;
        virtual bool AdaptForVideoMode(const VideoModeDescriptor& videoModeDesc) override;
        virtual Display::UniquePtr GetResidentDisplay() const override;

        inline Texture::Ptr GetTarget() const { return mTarget; }

    protected:
        OffscreenSurface(int width, int height);

    private:
        Texture::Ptr mTarget;
    };
}