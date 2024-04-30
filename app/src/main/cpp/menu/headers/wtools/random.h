//
// Created by sel on 30/04/2024.
//

#include "../../../global_includes/ginc.h"

#ifndef HARD_MOTION_RANDOM_H
#define HARD_MOTION_RANDOM_H

namespace random_
{
    template <typename T = std::size_t>
    inline T random_num(T min, T max) noexcept
    {
        thread_local std::default_random_engine generator(std::random_device{}());
        std::uniform_int_distribution<T> distribution(min, max);
        return distribution(generator);
    }

    template <typename T = std::size_t>
    inline T random_num(T max) noexcept
    {
        return random_num<T>(std::numeric_limits<T>::lowest(), max);
    }

    template <typename T = std::size_t>
    inline std::chrono::milliseconds random_time_delay(T min_seconds, T max_seconds) noexcept
    {
        return std::chrono::milliseconds(random_num(min_seconds, max_seconds) * 1000);
    }

    std::string random_str(std::string::size_type length) noexcept;
}


#endif //HARD_MOTION_RANDOM_H
