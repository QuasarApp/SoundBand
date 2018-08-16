#ifndef CHRONOTIME_H
#define CHRONOTIME_H
#include <chrono>
#include "config.h"

/**
 * @brief Time_point on nanosecunds (uint64_t)
 */
typedef long long milliseconds;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> Clock;

class ChronoTime
{
public:
    ChronoTime();
    /**
     * @brief now - get now time on microsecunds
     * @return - count of microsecunds
     */
    static milliseconds now(milliseconds calibration = 0);
    /**
     * @brief from cast to chrono secunds
     * @param mcrs microseconds of uint_64
     * @return microseconds of chrono
     */
    static Clock from(const milliseconds &mcrs);

    /**
     * @brief abs
     * @return module of numver
     */
    static milliseconds abs(milliseconds number);
};

#endif // CHRONOTIME_H
