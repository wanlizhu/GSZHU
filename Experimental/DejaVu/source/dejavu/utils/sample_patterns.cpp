#include "utils/sample_patterns.h"
#include "utils/logger.h"

namespace djv
{
    int DXSamplePattern::kSampleCount = 8;
    float2 DXSamplePattern::kPattern[DXSamplePattern::kSampleCount] = {
        {  1.0f / 16.0f, -3.0f / 16.0f },
        { -1.0f / 16.0f,  3.0f / 16.0f },
        {  5.0f / 16.0f,  1.0f / 16.0f },
        { -3.0f / 16.0f, -5.0f / 16.0f },
        { -5.0f / 16.0f,  5.0f / 16.0f },
        { -7.0f / 16.0f, -1.0f / 16.0f },
        {  3.0f / 16.0f,  7.0f / 16.0f },
        {  7.0f / 16.0f, -7.0f / 16.0f }
    };

    float2 HaltonSamplePattern::kPattern[8] = {
        { 1.0f / 2.0f - 0.5f, 1.0f / 3.0f - 0.5f },
        { 1.0f / 4.0f - 0.5f, 2.0f / 3.0f - 0.5f },
        { 3.0f / 4.0f - 0.5f, 1.0f / 9.0f - 0.5f },
        { 1.0f / 8.0f - 0.5f, 4.0f / 9.0f - 0.5f },
        { 5.0f / 8.0f - 0.5f, 7.0f / 9.0f - 0.5f },
        { 3.0f / 8.0f - 0.5f, 2.0f / 9.0f - 0.5f },
        { 7.0f / 8.0f - 0.5f, 5.0f / 9.0f - 0.5f },
        { 0.5f / 8.0f - 0.5f, 8.0f / 9.0f - 0.5f }
    };

    int DXSamplePattern::sampleCount() const
    {
        return kSampleCount;
    }

    void DXSamplePattern::reset(int start)
    {
        mCurrentSample = start;
    }

    float2 DXSamplePattern::next()
    {
        return kPattern[(mCurrentSample++) % kSampleCount];
    }

    HaltonSamplePattern::HaltonSamplePattern(uint32_t sampleCount)
    {
        LOG_IF(WARNING, sampleCount > 8) << "HaltonSamplePattern() requires sampleCount in the range [1, 8]. Clamping to that range.";
        mSampleCount = std::max(1, std::min(8, sampleCount));
    }

    uint32_t HaltonSamplePattern::sampleCount() const
    {
        return mSampleCount;
    }

    void HaltonSamplePattern::reset(uint32_t start)
    {
        mCurrentSample = start;
    }

    float2 HaltonSamplePattern::next()
    {
        return return kPattern[(mCurrentSample++) % mSampleCount];;
    }

    StratifiedSamplePattern::StratifiedSamplePattern(uint32_t sampleCount)
    {
        LOG_IF(WARNING, sampleCount < 1) << "StratifiedSamplePattern() requires sampleCount > 0. Using 1 sample.";
        LOG_IF(WARNING, sampleCount > 1024) << "StratifiedSamplePattern() requires sampleCount <= 1024. Using 1024 samples.";
        
        // Clamp sampleCount to a reasonable number so the permutation table doesn't get too big.
        sampleCount = std::clamp(sampleCount, 1, 1024);

        // Factorize sampleCount into an M x N grid, where M and N are as close as possible.
        // In the worst case sampleCount is prime and we'll end up with a sampleCount x 1 grid.
        mBinsX = (uint32_t)std::sqrt((double)sampleCount);
        mBinsY = sampleCount / mBinsX;
        while (mBinsX * mBinsY != sampleCount)
        {
            mBinsX++;
            mBinsY = sampleCount / mBinsX;
        }
        assert(mBinsX * mBinsY == sampleCount);

        // Create permutation table.
        mPermutation.resize(sampleCount);
        for (uint32_t i = 0; i < sampleCount; i++) {
            mPermutation[i] = i;
        }
    }

    uint32_t StratifiedSamplePattern::sampleCount() const
    {
        return mBinsX * mBinsY;
    }

    void StratifiedSamplePattern::reset(uint32_t start)
    {
        LOG_IF(WARNING, start > 0) << "StratifiedSamplePattern::reset() doesn't support restarting at an arbitrary sample. Using startID = 0.";
        mCurrentSample = 0;
        mRandomGenerator = std::mt19937();
    }

    float2 StratifiedSamplePattern::next()
    {
        auto dist = std::uniform_real_distribution<float>();
        auto u = [&]() { return dist(mRandomGenerator); };

        // Create new permutation at the start of each round of sampling.
        if (mCurrentSample == 0) {
            std::shuffle(mPermutation.begin(), mPermutation.end(), mRandomGenerator);
        }

        // Compute stratified point in the current bin.
        uint32_t binIdx = mPermutation[mCurrentSample];
        uint32_t i = binIdx % mBinsX;
        uint32_t j = binIdx / mBinsX;
        mCurrentSample = (mCurrentSample + 1) % sampleCount();

        assert(i < mBinsX && j < mBinsY);
        float x = ((float)i + u()) / mBinsX;
        float y = ((float)j + u()) / mBinsY;
        return float2(x, y) - 0.5f;
    }
}