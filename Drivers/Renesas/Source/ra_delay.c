#include "R7FA6M5BH.h"
#include "ra_delay.h"

extern uint32_t SystemCoreClock;
extern volatile uint32_t uwTick;

/**
 * @brief 软件延迟函数
 * @param delay 延迟值
 * @param units 延迟单位 (秒/毫秒/微秒)
 * @note 对于微秒级延迟，使用粗略的循环实现；毫秒/秒级使用 uwTick 计时
 */
void R_BSP_SoftwareDelay(uint32_t delay, delay_units_t units)
{
    if (delay == 0)
    {
        return;  // 延迟0直接返回
    }

    switch (units)
    {
    case DELAY_UNITS_MICROSECONDS:
        {
            // 微秒级延迟 - 使用粗略的循环实现
            // 假设系统主频为200MHz时，每个循环约消耗5个时钟周期
            uint32_t cycles_per_us = SystemCoreClock / 1000000 / 5;
            uint32_t total_cycles = delay * cycles_per_us;

            for (volatile uint32_t i = 0; i < total_cycles; i++)
            {
                __NOP();  // 空操作，防止被优化掉
            }
            break;
        }

    case DELAY_UNITS_MILLISECONDS:
        {
            // 毫秒级延迟 - 使用uwTick
            uint32_t start_tick = uwTick;
            while ((uwTick - start_tick) < delay)
            {
                // 防止uwTick溢出
                if (uwTick < start_tick)
                {
                    break;
                }
            }
            break;
        }

    case DELAY_UNITS_SECONDS:
        {
            // 秒级延迟 - 转换为毫秒处理
            uint32_t milliseconds = delay * 1000;
            uint32_t start_tick = uwTick;
            while ((uwTick - start_tick) < milliseconds)
            {
                // 防止uwTick溢出
                if (uwTick < start_tick)
                {
                    break;
                }
            }
            break;
        }
    }
}