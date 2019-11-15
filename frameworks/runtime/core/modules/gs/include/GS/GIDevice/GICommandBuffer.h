#pragma once

#include "GIDeviceObject.h"

namespace GS
{
    class GICommandBuffer : public GIDeviceObject
    {
    public:
        using Ptr = std::shared_ptr<GICommandBuffer>;

        static Ptr Create(const std::string& name, GIDevice::Ptr device);

        // virtual methods from GIDeviceObject
        virtual void Init(void* data) override;
        virtual void Destroy() override;

        virtual void AddCmdCopy() const = 0;
        virtual void AddCmdSet() const = 0;
        virtual void AddCmdBarrier() const = 0;
        virtual void AddCmdWait() const = 0;
        virtual void AddCmdClear() const = 0;
        virtual void AddCmdDraw() const = 0;
        virtual void AddCmdDispatch() const = 0;
        virtual void AddCmdQuery() const = 0;
        virtual void AddCmdCall() const = 0;

        bool Open();
        void Close();
        void Clear();
        void Commit() const;
        bool IsOpen() const;

    public:
        Signal<void(void*)> onCompleted;
    };
}