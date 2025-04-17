//
// Created by TaterLi on 25-4-16.
//

#include "R7FA6M5BH.h"
#include "ra_register_protection.h"

/** 用于保护位引用计数器的存储 */
volatile uint16_t g_protect_counters[4];

/** 用于设置或清除PRCR寄存器的掩码。对于MPC中的PWPR使用方式不同，因此用-1表示大小 */
static const uint16_t g_prcr_masks[] =
{
    0x0001U,                           /* PRC0保护位 */
    0x0002U,                           /* PRC1保护位 */
    0x0008U,                           /* PRC3保护位 */
    0x0010U,                           /* PRC4保护位 */
};

/*******************************************************************************************************************//**
 *        启用寄存器保护。被保护的寄存器不可写入。寄存器保护通过保护寄存器(PRCR)和MPC的写保护寄存器(PWPR)启用。
 *
 * @param[in] regs_to_protect 需要启用写保护的寄存器
 **********************************************************************************************************************/
void R_BSP_RegisterProtectEnable (reg_protect_t regs_to_protect)
{
    /** 获取/保存当前中断状态 */
    uint32_t primask = __get_PRIMASK();
    __set_PRIMASK((1U));

    /* 检查是否可以安全禁用写访问 */
    if (0U != g_protect_counters[regs_to_protect])
    {
        /* 减少保护计数器 */
        g_protect_counters[regs_to_protect]--;
    }

    /* 检查是否可以安全禁用写访问 */
    if (0U == g_protect_counters[regs_to_protect])
    {
        /** 通过PRCR寄存器启用保护
         *
         * 写入PRCR寄存器时，高8位必须是正确的密钥。将低位设为0以禁用写入 */
        R_SYSTEM->PRCR = ((R_SYSTEM->PRCR | 0xA500) & (uint16_t) (~g_prcr_masks[regs_to_protect]));
    }

    /** 恢复中断状态 */
    __set_PRIMASK(primask);
}

/*******************************************************************************************************************//**
 *        禁用寄存器保护。被保护的寄存器不可写入。寄存器保护通过保护寄存器(PRCR)和MPC的写保护寄存器(PWPR)禁用。
 *
 * @param[in] regs_to_unprotect 需要禁用写保护的寄存器
 **********************************************************************************************************************/
void R_BSP_RegisterProtectDisable (reg_protect_t regs_to_unprotect)
{
    /** 获取/保存当前中断状态 */
    uint32_t primask = __get_PRIMASK();
    __set_PRIMASK((1U));

    /* 如果是首次进入，则禁用保护 */
    if (0U == g_protect_counters[regs_to_unprotect])
    {
        /** 通过PRCR寄存器禁用保护
         *
         * 写入PRCR寄存器时，高8位必须是正确的密钥。设置低位以允许写入 */
        R_SYSTEM->PRCR = ((R_SYSTEM->PRCR | 0xA500) | g_prcr_masks[regs_to_unprotect]);
    }

    /** 增加保护计数器 */
    g_protect_counters[regs_to_unprotect]++;

    /** 恢复中断状态 */
    __set_PRIMASK(primask);
}