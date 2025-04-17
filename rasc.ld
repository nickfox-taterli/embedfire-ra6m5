/* 程序入口点 */
ENTRY(Reset_Handler)

/* 存储器定义 */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 128K    /* RAM：可执行/可读/可写，起始地址0x20000000，长度128KB */
  FLASH  (rx)     : ORIGIN = 0x00000000,   LENGTH = 2M      /* FLASH：只读/可执行，起始地址0x00000000，长度2MB */
}

/* 段(Section)定义 */
SECTIONS
{
  /* 启动代码存放在FLASH中 */
  .isr_vector :
  {
    . = ALIGN(4);                                          /* 4字节对齐 */
    KEEP(*(.fixed_vectors*))                               /* 固定向量表 */
    . = ALIGN(4);
  } >FLASH

  /* 程序代码和其他数据存放在FLASH中 */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* 代码段 */
    *(.text*)          /* 其他代码段 */
    *(.glue_7)         /* ARM/Thumb代码粘合段 */
    *(.glue_7t)        /* Thumb/ARM代码粘合段 */
    *(.eh_frame)       /* 异常处理帧信息 */

    KEEP (*(.init))    /* 保留初始化代码 */
    KEEP (*(.fini))    /* 保留终止代码 */

    . = ALIGN(4);
    _etext = .;        /* 定义代码结束的全局符号 */
  } >FLASH

  /* 常量数据存放在FLASH中 */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* 只读数据段(常量、字符串等) */
    *(.rodata*)        /* 其他只读数据段 */
    . = ALIGN(4);
  } >FLASH

  /* ARM异常处理表 */
  .ARM.extab () :
  {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >FLASH

  /* ARM索引表 */
  .ARM () :
  {
    . = ALIGN(4);
    __exidx_start = .;  /* 定义索引表开始 */
    *(.ARM.exidx*)
    __exidx_end = .;    /* 定义索引表结束 */
    . = ALIGN(4);
  } >FLASH

  /* 初始化数组 */
  .init_array () :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >FLASH

  /* 供启动代码初始化数据使用的符号 */
  _sidata = LOADADDR(.data);

  /* 初始化数据段(加载到FLASH但运行在RAM) */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* 定义数据段开始的全局符号 */
    *(.data)           /* 数据段 */
    *(.data*)          /* 其他数据段 */

    . = ALIGN(4);
    _edata = .;        /* 定义数据段结束的全局符号 */
  } >RAM AT> FLASH

  /* 未初始化数据段(BSS) */
  . = ALIGN(4);
  .bss :
  {
    _sbss = .;         /* 定义BSS段开始的全局符号 */
    __bss_start__ = _sbss;
    *(.bss)            /* BSS段 */
    *(.bss*)           /* 其他BSS段 */
    *(COMMON)          /* 公共符号 */

    . = ALIGN(4);
    _ebss = .;         /* 定义BSS段结束的全局符号 */
    __bss_end__ = _ebss;
  } >RAM

  /* 堆栈段(不加载) */
  .stack_dummy (NOLOAD):
  {
    . = ALIGN(8);
    /* 主堆栈 */
    KEEP(*(.stack))
  } > RAM

  /* 丢弃的段 */
  /DISCARD/ :
  {
    libc.a ( * )      /* 丢弃标准库 */
    libm.a ( * )      /* 丢弃数学库 */
    libgcc.a ( * )    /* 丢弃GCC库 */
  }

  /* ARM属性段 */
  .ARM.attributes 0 : { *(.ARM.attributes) }
}