#ifndef __RANDOM_ENGINE_HPP__
#define __RANDOM_ENGINE_HPP__

#include <random>
#include <functional>
#include <type_traits>

namespace Random
{
    template <typename T, typename D = typename std::conditional<std::is_integral<T>::value, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>::type>
    inline auto getEngine(
        const T &min = std::numeric_limits<T>::min(),
        const T &max = std::numeric_limits<T>::max())
    {
        return std::bind(D(min, max), std::mt19937{std::random_device{}()});
    }
}

#endif