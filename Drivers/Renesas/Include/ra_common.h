//
// Created by TaterLi on 25-4-16.
//

#ifndef RA_COMMON_H
#define RA_COMMON_H

#include "R7FA6M5BH.h"

__STATIC_INLINE void R_BSP_FlashCacheEnable (void)
{
    /* 使闪存缓存无效并等待操作完成（详见RA6M3手册R01UH0878EJ0100第55.3.2.2节"操作"说明） */
    R_FCACHE->FCACHEIV = 1U;
    while (R_FCACHE->FCACHEIV != 0U) {};

    /* 使能闪存缓存 */
    R_FCACHE->FCACHEE = 1U;

    /* 配置C-Cache行大小 */
    R_CACHE->CCALCF = 1;

    /* 使能C-Cache */
    R_CACHE->CCACTL = 1U;
}

#endif //RA_COMMON_H
