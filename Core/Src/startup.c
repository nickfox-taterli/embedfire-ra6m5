#include "R7FA6M5BH.h"
#include "ra_common.h"

uint32_t SystemCoreClock;  // 系统核心时钟频率变量

// 主栈空间定义（1KB大小，8字节对齐，放在stack段）
static uint8_t g_main_stack[0x400] __attribute__((aligned(8))) __attribute__((section("stack"))) __attribute__((__used__));

// 异常处理函数声明
void Reset_Handler(void);
void Default_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SecureFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

typedef void (* exc_ptr_t)(void);  // 异常处理函数指针类型定义

// 中断向量表定义
__attribute__((used)) const exc_ptr_t __Vectors[] __attribute__((section(".fixed_vectors"))) __attribute__((__used__)) =
{
    (exc_ptr_t) (&g_main_stack[0] + 0x400),  // 初始栈指针
    Reset_Handler,                           // 复位处理函数
    NMI_Handler,                             // NMI中断处理函数
    HardFault_Handler,                       // 硬件错误处理函数
    MemManage_Handler,                       // 内存管理错误处理函数
    BusFault_Handler,                        // 总线错误处理函数
    UsageFault_Handler,                      // 用法错误处理函数
    SecureFault_Handler,                     // 安全错误处理函数
    0,                                       // 保留
    0,                                       // 保留
    0,                                       // 保留
    SVC_Handler,                             // SVC调用处理函数
    DebugMon_Handler,                        // 调试监控处理函数
    0,                                       // 保留
    PendSV_Handler,                          // PendSV处理函数
    SysTick_Handler,                         // SysTick处理函数
};

// 系统初始化函数
void SystemInit(void)
{
    /* 启用FPU */
    SCB->CPACR = (uint32_t)(0xFU << 20);

    /* 设置向量表偏移寄存器 */
    SCB->VTOR = (uint32_t)&__Vectors;

    /* 设置主栈指针限制 */
    __set_MSPLIM((uint32_t)&g_main_stack[0]);

    /* 清零.bss段（未初始化的全局变量区） */
    extern uint32_t __bss_start__;
    extern uint32_t __bss_end__;

    uint32_t *bss_start = &__bss_start__;
    uint32_t *bss_end = &__bss_end__;
    uint32_t bss_size = (uint32_t)bss_end - (uint32_t)bss_start;

    for(uint32_t i = 0; i < bss_size; i++) {
        *((uint8_t*)bss_start + i) = 0;
    }

    /* 初始化.data段（已初始化的全局变量区） */
    extern uint32_t _sidata;
    extern uint32_t _sdata;
    extern uint32_t _edata;

    uint32_t *src = &_sidata;  /* Flash中的数据源地址 */
    uint32_t *dest = &_sdata;  /* RAM中的目标地址 */
    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;

    for (uint32_t i = 0; i < size; i += 4) {
        *dest++ = *src++;
    }

    /* 调用全局构造函数 */
    extern void (*__init_array_start[])(void);
    extern void (*__init_array_end[])(void);

    int32_t count = __init_array_end - __init_array_start;
    for (int32_t i = 0; i < count; i++) {
        __init_array_start[i]();
    }

    /* 解锁CGC和LPM保护寄存器 */
    R_SYSTEM->PRCR = (0xA500U | 0x3U);

    /* 启用闪存缓存，运行时不关闭 */
    R_BSP_FlashCacheEnable();

    /* 配置主振荡器驱动 */
    R_SYSTEM->MOMCR = 0;

    /* 设置XTAL稳定时间 */
    R_SYSTEM->MOSCWTCR = 9;
    R_SYSTEM->MOSCCR = 0U;

    /* 等待主振荡器稳定 */
    while (R_SYSTEM->OSCSF_b.MOSCSF != 1U) {}

    /* 配置PLL到200MHz */
    R_SYSTEM->PLLCCR = 0x3102;

    R_SYSTEM->PLLCR = 0U;

    /* 等待PLL稳定 */
    while (R_SYSTEM->OSCSF_b.PLLSF != 1U) {}

    /* 配置闪存等待周期 */
    R_FCACHE->FLWT = 3;

    /* 设置时钟分频 */
    R_SYSTEM->SCKDIVCR = 0x20011221;

    /* 系统切换到PLL时钟 */
    R_SYSTEM->SCKSCR = 5;

    /* 更新系统核心时钟变量 */
    SystemCoreClock = 200000000;

    /* 调整SRAM等待状态 */
    R_SRAM->SRAMPRCR2 = 0x79;
    R_SRAM->SRAMWTSC  = 1;
    R_SRAM->SRAMPRCR2 = 0x78;

    /* 配置备份时钟 */
    R_SYSTEM->BCKCR   = 1;
    R_SYSTEM->EBCKOCR = 1;

    /* 将所有中断向量设置为非安全状态 */
    R_CPSCU->ICUSARG = 0xFFFFFFFF;
    R_CPSCU->ICUSARH = 0xFFFFFFFF;
    R_CPSCU->ICUSARI = 0xFFFFFFFF;

    NVIC->ITNS[0] = 0xFFFFFFFF;
    NVIC->ITNS[1] = 0xFFFFFFFF;
    NVIC->ITNS[2] = 0xFFFFFFFF;

    /* 关键寄存器上锁 */
    R_SYSTEM->PRCR = (0xA500U) | 0x0U;

    SysTick->LOAD = (SystemCoreClock / 1000) - 1;      // 设置重装载值
    SysTick->VAL = 0;                              // 清除当前值
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |  // 使用处理器时钟
                    SysTick_CTRL_TICKINT_Msk   |  // 使能中断
                    SysTick_CTRL_ENABLE_Msk;      // 使能SysTick
}

// 默认异常处理函数
void Default_Handler(void)
{
    /* 发生错误，触发断点以便调试 */
    __BKPT((0));
}

// 复位处理函数
void Reset_Handler(void)
{
    /* 初始化系统 */
    SystemInit();

    /* 调用用户主函数 */
    extern void main(void);
    main();

    /* 主函数不应返回，如果返回则进入死循环 */
    while (1) {}
}