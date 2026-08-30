# OV-Watch IAP_F411工程详解

## 工程定位

这是 OV-Watch 的 STM32F411 IAP（In-Application Programming，应用内编程）工程。它负责：

- 上电后判断是否进入升级菜单
- 通过 USART1 接收 Ymodem 文件
- 擦除并写入 APP 区域的内部 Flash
- 下载成功后写入 `APP FLAG`
- 根据 APP 向量表跳转到用户应用程序

Keil 工程文件在当前 `MDK-ARM` 目录，但源码通过 `../` 引用上一级目录：

|目录|主要职责|
|---|---|
|`../Core/Src`|CubeMX 生成的启动、时钟、GPIO、USART、SPI、TIM、ADC 和中断代码|
|`../BSP`|按键、LCD、KT6328 蓝牙、电源等板级驱动|
|`../SYSTEM`|延时函数|
|`../Ymodem`|IAP 菜单、串口命令、Ymodem 协议和 Flash 操作|
|`IAP_F411.uvprojx`|Keil 编译、链接、下载配置|

## Flash地址规划

```text
0x08000000  ┌──────────────────────────────┐
            │ IAP 启动程序                  │
0x0800BFFF  └──────────────────────────────┘
0x0800C000  ┌──────────────────────────────┐
            │ APP 向量表和 APP 程序          │  APPLICATION_ADDRESS
            │                                │
            │  第 0 个字：初始 MSP           │
            │  第 1 个字：Reset_Handler      │
            │                                │
0x0807FFFF  └──────────────────────────────┘
```

本工程还把 APP 标志写在 `0x08008000`（Sector 2 起始地址）。下载开始时先擦除该扇区；只有 Ymodem 完整接收成功后，才写入两个 32 位字组成的字符串 `APP FLAG`。

## 上电运行流程

%%{init:{'theme':'default','themeVariables':{'fontSize':'12px'}}}%%
flowchart TD
    A([复位]):::reset --> B[main]
    B --> C[HAL_Init和SystemClock_Config]:::init
    C --> D[初始化GPIO USART1 SPI1 TIM3 ADC1以及板级驱动]:::init
    D --> E{KEY1是否持续按下?}:::decision
    E -->|是| F[擦写接口初始化]:::iap
    F --> G[Main_Menu阻塞等待串口菜单]:::iap
    G --> H{菜单操作}
    H -->|1| I[擦除APP区并接收Ymodem]:::flash
    I --> J{接收成功?}:::decision
    J -->|是| K[写入APP FLAG]:::flash
    J -->|否| G
    K --> G
    H -->|3| L[读取APP向量表并跳转]:::jump
    E -->|否| M[读取0x08008000标志]:::check
    M --> N{等于APP FLAG?}:::decision
    N -->|是| L
    N -->|否| O[LCD提示No App并停留IAP]:::error
    classDef reset fill:#ffe6e6,stroke:#c0392b,color:#222;
    classDef init fill:#e8f4ff,stroke:#2980b9,color:#222;
    classDef decision fill:#fff4cc,stroke:#f39c12,color:#222;
    classDef iap fill:#e8f8f0,stroke:#27ae60,color:#222;
    classDef flash fill:#f3e8ff,stroke:#8e44ad,color:#222;
    classDef jump fill:#dff6ff,stroke:#16a085,color:#222;
    classDef check fill:#fef0e6,stroke:#d35400,color:#222;
    classDef error fill:#ffe6e6,stroke:#c0392b,color:#222;

## APP跳转的关键原理

APP 不是简单的“调用一个地址”。Cortex-M 向量表开头是两个特殊数据：

```c
uint32_t app_msp   = *(uint32_t *)APPLICATION_ADDRESS;
uint32_t app_reset = *(uint32_t *)(APPLICATION_ADDRESS + 4U);
```

跳转代码依次完成：

- 停止 IAP 的 SysTick，避免 APP 继承错误的节拍中断
- 清零 SysTick 重载值和当前值
- 禁止全局中断
- 读取 APP 的 `Reset_Handler`
- 用 APP 向量表第 0 项设置 MSP
- 调用 `Reset_Handler`

APP 工程还必须把中断向量表放到 `0x0800C000`，通常需要在 APP 启动阶段设置 `SCB->VTOR = APPLICATION_ADDRESS`；否则 APP 的中断可能仍索引 IAP 的向量表。

## Ymodem接收链路

```text
主机发送文件
    ↓
Receive_Byte轮询USART1 RXNE
    ↓
Receive_Packet识别SOH/STX/EOT/CA
    ↓
校验包序号及其反码
    ↓
第0包解析文件名和文件大小
    ↓
检查大小并擦除APP区
    ↓
数据包复制到tab_1024
    ↓
FLASH_If_Write按32位字编程并回读
    ↓
EOT后ACK，返回文件大小
    ↓
SerialDownload写入APP FLAG
```

Ymodem 控制字符含义：

|宏|值|作用|
|---|---:|---|
|`SOH`|`0x01`|128 字节数据包|
|`STX`|`0x02`|1024 字节数据包|
|`EOT`|`0x04`|文件传输结束|
|`ACK`|`0x06`|接收正确|
|`NAK`|`0x15`|请求重发|
|`CA`|`0x18`|连续两个表示取消|
|`CRC16`|`0x43`|发送字符 `C`，请求 CRC 模式|

## Flash写入的两个单位

需要特别区分：

- Ymodem 的 `packet_length` 单位是字节，例如 `1024`
- `FLASH_If_Write()` 的 `DataLength` 单位是 32 位字，例如 `1024 / 4 = 256`

Flash 写入地址必须满足 4 字节对齐，镜像长度也最好满足 4 字节对齐。若文件长度不是 4 的整数倍，当前代码的尾包处理需要结合发送端实际填充规则验证，不能仅凭编译通过判断可靠。

## 重要工程检查点

### 标志地址与应用地址不同

当前实现把 `APP FLAG` 写到 `0x08008000`，APP 从 `0x0800C000` 开始。不要把这两个地址混为一谈。标志用于“是否允许跳转”，向量表用于“跳转到哪里以及使用哪个栈”。

### APP_FLAG写入方式

`SerialDownload()` 中把字符串常量地址转换为 `uint32_t *`，再写入 Flash。该做法依赖字符串常量的内存布局和 4 字节可读性，工程上更稳妥的做法是使用明确的 `uint8_t app_flag[8]` 或两个 `uint32_t` 常量；本次仅添加注释，没有改变原有行为。

### CRC完整性

当前 `Receive_Packet()` 明确检查了包序号和反码，但需要结合完整函数继续确认 CRC16 是否在接收端实际比较。下载验证时应使用串口抓包或故意损坏数据包进行测试，确认错误包不会被写入 Flash。

### 验证边界

本次修改属于注释和学习文档增强，没有运行 Keil 编译、仿真、烧录或真实串口 Ymodem 测试。因此：

- 代码行为未被有意改变
- `git diff --check` 只能说明差异格式基本正常
- 不能据此宣称 IAP 下载、APP 跳转或硬件显示已经验证通过

## 建议阅读顺序

建议按以下顺序理解源码：

```text
main.c
  → menu.c / menu.h
    → flash_if.c / flash_if.h
    → ymodem.c / ymodem.h
      → common.c / common.h
  → usart.c和stm32f4xx_it.c
  → BSP下的LCD、KEY、POWER和KT6328
```

先掌握启动路径和地址规划，再阅读协议细节，最后看外围显示和按键，最容易建立完整的运行时心智模型。
