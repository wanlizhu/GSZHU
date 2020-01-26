#include "RHI/Class/CTextureBase.h"
#include "Utils/Logger.h"
#include "RHI/ITextureView.h"

namespace Wanlix
{
    SharedPtr<ITextureView> CTextureBase::CreateView(const TextureViewDesc& viewDesc)
    {
        CHECK_ERROR(viewDesc.viewType != ETextureViewType::Undefined, 
                    "Texture view type is not specified");
        
        if (viewDesc.viewType == ETextureViewType::ShaderResource)
        {
            CHECK_ERROR(HasBits(mDesc.bindFlags, EBindFlags::ShaderResource),
                        "Attempting to create SRV for texture that was not created with EBindFlags::ShaderResource flag");
        }
        else if (viewDesc.viewType == ETextureViewType::UnorderedAccess)
        {
            CHECK_ERROR(HasBits(mDesc.bindFlags, EBindFlags::UnorderedAccess),
                        "Attempting to create UAV for texture that was not created with BIND_UNORDERED_ACCESS flag");
        }
        else if (viewDesc.viewType == ETextureViewType::RenderTarget)
        {
            CHECK_ERROR(HasBits(mDesc.bindFlags, EBindFlags::RenderTarget), 
                        "Attempting to create RTV for texture that was not created with BIND_RENDER_TARGET flag");
        }
        else if (viewDesc.viewType == ETextureViewType::DepthStencil)
        {
            CHECK_ERROR(HasBits(mDesc.bindFlags, EBindFlags::DepthStencil),
                        "Attempting to create DSV for texture that was not created with BIND_DEPTH_STENCIL flag");
        }
        else
        {
            UNEXPECTED("Unexpected texture view type");
        }

        return CreateViewInternal(viewDesc);
    }

    SharedPtr<ITextureView> CTextureBase::GetDefaultView(ETextureViewType viewType)
    {
        switch (viewType)
        {
            case ETextureViewType::ShaderResource:  return mDefaultSRV;
            case ETextureViewType::RenderTarget:    return mDefaultRTV;
            case ETextureViewType::DepthStencil:    return mDefaultDSV;
            case ETextureViewType::UnorderedAccess: return mDefaultUAV;
            default: UNEXPECTED("Unknown view type"); return nullptr;
        }
    }

    void CTextureBase::SetState(EResourceState state)
    {
        assert(mSubresourceStates.empty());
        if (mSubresourceStates.empty())
        {
            mResourceState = state;
        }
    }

    void CTextureBase::SetSubresourceState(Uint mipLevel, Uint slice, EResourceState state)
    {
        mSubresourceStates[Subresource(mipLevel, slice)] = state;
    }

    Optional<EResourceState> CTextureBase::GetState() const
    {
        return mResourceState;
    }

    Optional<EResourceState> CTextureBase::GetSubresourceState(Uint mipLevel, Uint slice) const
    {
        Subresource subres(mipLevel, slice);
        auto it = mSubresourceStates.find(subres);
        if (it == mSubresourceStates.end())
        {
            return std::nullopt;
        }
        return it->second;
    }

    Bool CTextureBase::IsInKnownState() const
    {
        return mResourceState == EResourceState::Unknown;
    }

    Bool CTextureBase::CheckState(EResourceState state) const
    {
        CHECK_ERROR(IsInKnownState(), "Texture state is unknown");
        return mResourceState == state;
    }
}