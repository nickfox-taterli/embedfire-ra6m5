#include "R7FA6M5BH.h"
#include "ra_delay.h"

/**
 * @brief 主函数
 *
 * 1. 解除端口保护
 * 2. 配置P4.0为输出模式
 * 3. 进入无限循环，周期性翻转P4.0电平
 */
void main(void)
{
    /* 解除端口写保护 */
    R_PMISC->PWPR = 0;                // 先写入0解除保护
    R_PMISC->PWPR = 1U << 6;          // 再设置PFSWE位允许端口配置

    /* 配置P4.0引脚为通用输出模式 */
    R_PFS->PORT[4].PIN[0].PmnPFS = 0x04;

    /* 主循环 - LED闪烁控制 */
    while(1)
    {
        // 使用异或操作翻转P4.0电平(PODR的第0位对应P4.0)
        R_PORT4->PODR ^= 0x01;

        // 延时1000毫秒(1秒)
        R_BSP_SoftwareDelay(1000, DELAY_UNITS_MILLISECONDS);
    }
}

