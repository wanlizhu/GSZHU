#pragma once

#include "ISurface.h"
#include "ITexture.h"

namespace Wanlix
{
    class OffscreenSurface : public ISurface
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
        virtual IDisplay::UniquePtr GetResidentDisplay() const override;

        inline ITexture::Ptr GetTarget() const { return mTarget; }

    protected:
        OffscreenSurface(int width, int height);

    private:
        ITexture::Ptr mTarget;
    };
}