#pragma once

#include <memory>
#include <string>
#include <random>
#include "utils/preprocess.h"
#include "utils/algebra.h"
#include "utils/inherit_shared_from_this.h"

namespace djv
{
    class DJV_API SamplePatternInterface : public std::enable_shared_from_this<SamplePattern>
    {
    public:
        virtual ~SamplePatternInterface() = default;
        virtual uint32_t sampleCount() const = 0;
        virtual void     reset(int start = 0) = 0;
        virtual float2   next() = 0;
    };

    class DJV_API DXSamplePattern 
        : public SamplePatternInterface
        , public inherit_shared_from_this<SamplePatternInterface, DXSamplePattern>
    {
    public:
        using inherit_shared_from_this<SamplePatternInterface, DXSamplePattern>::shared_from_this;
        static constexpr uint32_t kSampleCount = 8;
        static constexpr float2 kPattern[kSampleCount];
        
        DXSamplePattern() = default;

        virtual ~DXSamplePattern() = default;
        virtual uint32_t sampleCount() const override;
        virtual void     reset(uint32_t start = 0) override;
        virtual float2   next() override;

    protected:
        uint32_t mCurrentSample = 0;
    };

    class DJV_API HaltonSamplePattern 
        : public SamplePatternInterface
        , public inherit_shared_from_this<SamplePatternInterface, HaltonSamplePattern>
    {
    public:
        using inherit_shared_from_this<SamplePatternInterface, HaltonSamplePattern>::shared_from_this;
        static constexpr float2 kPattern[8];
        
        HaltonSamplePattern(uint32_t sampleCount);

        virtual ~HaltonSamplePattern() = default;
        virtual uint32_t sampleCount() const override;
        virtual void     reset(uint32_t start = 0) override;
        virtual float2   next() override;

    protected:
        uint32_t mCurrentSample = 0;
        uint32_t mSampleCount = 0;
    };

    class DJV_API StratifiedSamplePattern 
        : public SamplePatternInterface
        , public inherit_shared_from_this<SamplePatternInterface, StratifiedSamplePattern>
    {
    public:
        using inherit_shared_from_this<SamplePatternInterface, StratifiedSamplePattern>::shared_from_this;

        StratifiedSamplePattern(uint32_t sampleCount = 1);

        virtual ~HaltonSamplePattern() = default;
        virtual uint32_t sampleCount() const override;
        virtual void     reset(uint32_t start = 0) override;
        virtual float2   next() override;

    protected:
        uint32_t mCurrentSample = 0;
        uint32_t mBinsX = 0;
        uint32_t mBinsY = 0;
        std::mt19937 mRandomGenerator;
        std::vector<uint32_t> mPermutation;
    };
}