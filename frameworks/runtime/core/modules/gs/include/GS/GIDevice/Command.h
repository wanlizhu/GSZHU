#pragma once

#include "Common.h"
#include <any>

namespace GS::GI
{
    enum class ECommandType
    {
        None,
        Copy,
        Draw,
        Compute,
        Set,
        Wait,
        Barrier,
    };

    class Command
    {
        friend class CommandList;
    public:
        using Ptr = std::shared_ptr<Command>;

        Command() = default;
        Command(ECommandType type);

        ECommandType GetType() const;
        bool         IsEmpty() const;
        bool         HasParameter(const std::string& key) const;
        void         AddParameter(const std::string& key, const std::any& value);
        std::any     operator[](const std::string& key) const;

    private:
        ECommandType mType = ECommandType::None;
        std::unordered_map<std::string, std::any> mParameters;
    };

    Command::Ptr MakeCopyCommand(IResource::Ptr src, 
                                 IResource::Ptr dst,
                                 Range srcRange = Range(),
                                 Range dstRange = Range());
    Command::Ptr MakeCopyCommand(IResource::Ptr src,
                                 IResource::Ptr dst,
                                 Region srcRegion = Region(),
                                 Region dstRegion = Region());

    Command::Ptr MakeDrawCommand();
    Command::Ptr MakeComputeCommand();
    Command::Ptr MakeSetCommand();
    Command::Ptr MakeWaitCommand();
    Command::Ptr MakeBarrierCommand();
}