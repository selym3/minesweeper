#ifndef __RANDOM_ENGINE_HPP__
#define __RANDOM_ENGINE_HPP__

#include <random>
#include <functional>

namespace Random
{
    template <typename T>
    inline auto getEngine(
        const T &min = std::numeric_limits<T>::min(),
        const T &max = std::numeric_limits<T>::max())
    {

        // TODO: The twister state space is bigger than the seed returned by random_device
        std::mt19937 twister{std::random_device{}()};

        if constexpr (std::is_integral<T>::value)
            return std::bind(std::uniform_int_distribution<T>(min, max), twister);
        else
            return std::bind(std::uniform_real_distribution<T>(min, max), twister);

    }
}

#endif