#pragma once
#include "Debug.h"
#include <random>

class Random
{

  public:
    static int RandomRange(int min, int max, bool maxInclusive = true)
    {
        if (!mInitialized)
        {
            mGenerator.seed(mDevice());
            mInitialized = true;
        }

        std::uniform_int_distribution<std::mt19937::result_type> dist(min, maxInclusive ? max : (max - 1));
        return dist(mGenerator);
    }

    static int RandomRange(int min, int max, uint32_t seed, bool maxInclusive = true)
    {
        mGenerator.seed(seed);
        mInitialized = true;

        std::uniform_int_distribution<std::mt19937::result_type> dist(min, maxInclusive ? max : (max - 1));
        return dist(mGenerator);
    }

    static void Test()
    {
        for (int i = 0; i < 500; i++)
        {
            Debug::Log("Rand: {}", RandomRange(0, 100, false));
        }
    }

  private:
    inline static bool mInitialized = false;

    inline static std::random_device mDevice = {};
    inline static std::mt19937 mGenerator = {};
};
