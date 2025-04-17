//
// Created by TaterLi on 25-4-16.
//

#ifndef RA_DELAY_H
#define RA_DELAY_H

typedef enum
{
    DELAY_UNITS_SECONDS      = 1000000, ///< 秒级延迟
    DELAY_UNITS_MILLISECONDS = 1000,    ///< 毫秒级延迟
    DELAY_UNITS_MICROSECONDS = 1        ///< 微秒级延迟
} delay_units_t;

void R_BSP_SoftwareDelay (uint32_t delay, delay_units_t units);

#endif //RA_DELAY_H
