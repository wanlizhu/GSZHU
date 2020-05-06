#include "utils/sample_patterns.h"
#include "utils/logger.h"

namespace djv
{
    uint32_t DXSamplePattern::sampleCount() const
    {
        return kSampleCount;
    }

    void DXSamplePattern::reset(uint32_t start)
    {
        mCurrentSample = start;
    }

    glm::vec2 DXSamplePattern::next()
    {
        return kPattern[(mCurrentSample++) % kSampleCount];
    }

    HaltonSamplePattern::HaltonSamplePattern(uint32_t sampleCount)
    {
        LOG_IF(WARNING, sampleCount > 8) << "HaltonSamplePattern() requires sampleCount in the range [1, 8]. Clamping to that range.";
        mSampleCount = std::max(1u, std::min(8u, sampleCount));
    }

    uint32_t HaltonSamplePattern::sampleCount() const
    {
        return mSampleCount;
    }

    void HaltonSamplePattern::reset(uint32_t start)
    {
        mCurrentSample = start;
    }

    glm::vec2 HaltonSamplePattern::next()
    {
        return kPattern[(mCurrentSample++) % mSampleCount];
    }

    StratifiedSamplePattern::StratifiedSamplePattern(uint32_t sampleCount)
    {
        LOG_IF(WARNING, sampleCount < 1) << "StratifiedSamplePattern() requires sampleCount > 0. Using 1 sample.";
        LOG_IF(WARNING, sampleCount > 1024) << "StratifiedSamplePattern() requires sampleCount <= 1024. Using 1024 samples.";
        
        // Clamp sampleCount to a reasonable number so the permutation table doesn't get too big.
        sampleCount = std::clamp(sampleCount, 1u, 1024u);

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

    glm::vec2 StratifiedSamplePattern::next()
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
        return glm::vec2(x, y) - 0.5f;
    }
}