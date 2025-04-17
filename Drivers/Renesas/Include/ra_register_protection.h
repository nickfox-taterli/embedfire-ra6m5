//
// Created by TaterLi on 25-4-16.
//

#ifndef RA_REGISTER_PROTECTION_H
#define RA_REGISTER_PROTECTION_H

/** 可被保护的不同类型寄存器枚举 */
typedef enum e_bsp_reg_protect
{
    /** 允许写入时钟生成电路相关寄存器 */
    BSP_REG_PROTECT_CGC = 0,

    /** 允许写入操作模式、低功耗和电池备份功能相关寄存器 */
    BSP_REG_PROTECT_OM_LPC_BATT,

    /** 允许写入低电压检测(LVD)相关寄存器：
     * LVCMPCR, LVDLVLR, LVD1CR0, LVD1CR1, LVD1SR, LVD2CR0, LVD2CR1, LVD2SR */
    BSP_REG_PROTECT_LVD,

    /** 允许写入安全功能相关寄存器 */
    BSP_REG_PROTECT_SAR,
} reg_protect_t;

/**
 * @brief 启用寄存器写保护
 * @param[in] regs_to_protect 需要启用保护的寄存器类型
 */
void R_BSP_RegisterProtectEnable (reg_protect_t regs_to_protect);

/**
 * @brief 禁用寄存器写保护
 * @param[in] regs_to_unprotect 需要解除保护的寄存器类型
 */
void R_BSP_RegisterProtectDisable (reg_protect_t regs_to_unprotect);

#endif //RA_REGISTER_PROTECTION_H
