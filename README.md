# Minimal RA MCU Project / 极简瑞萨MCU项目

## Overview / 概述
A bare-metal minimal example for RA MCU (tested on RA6M5) with:
- Minimal CMake-based build system
- Minimal linker script
- Minimal startup code (only essential init & clock setup)
- Simple LED blink example

一个极简的RA MCU裸机示例(已在RA6M5测试)：
- 基于CMake的最小化构建系统
- 最小化链接脚本
- 最小化启动文件(仅基础初始化和时钟设置)
- 简单的LED闪烁示例

## Features / 特点
- **No RASC dependency** - 不依赖RASC工具链
- **Clean & transparent** - 没有自动生成的冗余代码
- **DIY-friendly** - 适合学习底层和自定义开发
- **Zero copyright** - 无版权限制，自由传播

## Trade-offs / 取舍
```
Pros/优势:
+ 完全掌控所有代码
+ 极简的代码结构
+ 快速编译和调试

Cons/局限:
- 需要手动实现外设驱动
- 缺少RASC的生态支持
- 需要熟悉底层寄存器操作
```

## Warning / 注意
This is a **hobbyist project** not suitable for production use. 
You'll need to:
- Manually adjust clock/GPIO config for your board
- Implement missing peripherals as needed
- Have basic knowledge of ARM Cortex-M architecture

这是一个**爱好者项目**，不适合生产环境。使用前需要：
- 根据开发板手动调整时钟/GPIO配置
- 自行实现所需外设驱动
- 具备ARM Cortex-M基础知识
