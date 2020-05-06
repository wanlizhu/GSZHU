#pragma once

#include <memory>
#include <string>
#include <random>
#include "utils/preprocess.h"
#include "utils/math3d.h"
#include "utils/inherit_shared_from_this.h"

namespace djv
{
    class DJV_API SamplePatternInterface : public std::enable_shared_from_this<SamplePatternInterface>
    {
    public:
        virtual ~SamplePatternInterface() = default;
        virtual uint32_t  sampleCount() const = 0;
        virtual void      reset(uint32_t start = 0) = 0;
        virtual glm::vec2 next() = 0;
    };

    class DJV_API DXSamplePattern 
        : public SamplePatternInterface
        , public inherit_shared_from_this<SamplePatternInterface, DXSamplePattern>
    {
    public:
        using inherit_shared_from_this<SamplePatternInterface, DXSamplePattern>::shared_from_this;
        static constexpr uint32_t  kSampleCount = 8;
        static constexpr glm::vec2 kPattern[kSampleCount] = {
            {  1.0f / 16.0f, -3.0f / 16.0f },
            { -1.0f / 16.0f,  3.0f / 16.0f },
            {  5.0f / 16.0f,  1.0f / 16.0f },
            { -3.0f / 16.0f, -5.0f / 16.0f },
            { -5.0f / 16.0f,  5.0f / 16.0f },
            { -7.0f / 16.0f, -1.0f / 16.0f },
            {  3.0f / 16.0f,  7.0f / 16.0f },
            {  7.0f / 16.0f, -7.0f / 16.0f }
        };
        
        DXSamplePattern() = default;

        virtual ~DXSamplePattern() = default;
        virtual uint32_t  sampleCount() const override;
        virtual void      reset(uint32_t start = 0) override;
        virtual glm::vec2 next() override;

    protected:
        uint32_t mCurrentSample = 0;
    };

    class DJV_API HaltonSamplePattern 
        : public SamplePatternInterface
        , public inherit_shared_from_this<SamplePatternInterface, HaltonSamplePattern>
    {
    public:
        using inherit_shared_from_this<SamplePatternInterface, HaltonSamplePattern>::shared_from_this;
        static constexpr glm::vec2 kPattern[8] = {
            { 1.0f / 2.0f - 0.5f, 1.0f / 3.0f - 0.5f },
            { 1.0f / 4.0f - 0.5f, 2.0f / 3.0f - 0.5f },
            { 3.0f / 4.0f - 0.5f, 1.0f / 9.0f - 0.5f },
            { 1.0f / 8.0f - 0.5f, 4.0f / 9.0f - 0.5f },
            { 5.0f / 8.0f - 0.5f, 7.0f / 9.0f - 0.5f },
            { 3.0f / 8.0f - 0.5f, 2.0f / 9.0f - 0.5f },
            { 7.0f / 8.0f - 0.5f, 5.0f / 9.0f - 0.5f },
            { 0.5f / 8.0f - 0.5f, 8.0f / 9.0f - 0.5f }
        };
        
        HaltonSamplePattern(uint32_t sampleCount);

        virtual ~HaltonSamplePattern() = default;
        virtual uint32_t  sampleCount() const override;
        virtual void      reset(uint32_t start = 0) override;
        virtual glm::vec2 next() override;

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

        virtual ~StratifiedSamplePattern() = default;
        virtual uint32_t  sampleCount() const override;
        virtual void      reset(uint32_t start = 0) override;
        virtual glm::vec2 next() override;

    protected:
        uint32_t mCurrentSample = 0;
        uint32_t mBinsX = 0;
        uint32_t mBinsY = 0;
        std::mt19937 mRandomGenerator;
        std::vector<uint32_t> mPermutation;
    };
}