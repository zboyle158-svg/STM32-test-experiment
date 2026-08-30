;*******************************************************************************
;* File Name          : startup_stm32f411xe.s
;* Author             : MCD Application Team
;* Description        : STM32F411xExx 的启动文件，适用于 Keil MDK-ARM。
;*                      该文件不是普通的应用逻辑，而是处理器复位后最早执行的
;*                      启动代码，主要负责：
;*                      - 为 MSP 提供初始栈顶地址；
;*                      - 建立异常和外部中断向量表；
;*                      - 从 Reset_Handler 进入 C 运行库 __main；
;*                      - 为未被用户工程实现的中断提供默认死循环；
;*                      - 向 C 运行库提供栈和堆的边界信息。
;*
;*                      Cortex-M4 复位后处于 Thread mode、特权级，使用主栈
;*                      MSP。向量表第 0 项由硬件装入 MSP，第 1 项作为复位入口。
;*******************************************************************************
;* @attention
;*
;* Copyright (c) 2017 STMicroelectronics.
;* All rights reserved.
;*
;* This software is licensed under terms that can be found in the LICENSE file
;* in the root directory of this software component.
;* If no LICENSE file comes with this software, it is provided AS-IS.
;*
;*******************************************************************************
;* <<< Use Configuration Wizard in Context Menu >>>
;
; 栈区大小，单位为字节。栈通常用于保存函数调用现场、局部变量、返回地址以及
; 中断现场。栈从高地址向低地址增长，因此 __initial_sp 位于 Stack_Mem 区域的
; 末端。栈不足会造成内存覆盖，表现为不可预测的 HardFault 或程序跑飞。
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size		EQU     0x400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp             ; 初始 MSP：向量表第 0 项必须指向栈顶，而不是栈底。


; 堆区大小，单位为字节。非 MicroLIB 配置下，C 运行库可通过这里返回的边界
; 管理 malloc/free 等动态内存；MicroLIB 配置下则直接导出堆边界符号。
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size      EQU     0x200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit             ; 堆区结束地址，供 C 运行库判断可用范围。

                PRESERVE8
                THUMB


; 向量表：每个 DCD 占 4 字节，表项内容是异常/中断处理函数地址。
; Cortex-M4 复位时从当前向量表读取：
;   [0] -> MSP 初值
;   [1] -> Reset_Handler 地址
; 随后的表项按 ARM Cortex-M 异常编号排列。若未重新设置 SCB->VTOR，
; 处理器默认从地址 0 取表；本工程的 IAP 启动映像通常位于 Flash 起始处。
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; [0] 初始 MSP，硬件复位时自动装入。
                DCD     Reset_Handler              ; Reset Handler
                DCD     NMI_Handler                ; NMI Handler
                DCD     HardFault_Handler          ; Hard Fault Handler
                DCD     MemManage_Handler          ; MPU Fault Handler
                DCD     BusFault_Handler           ; Bus Fault Handler
                DCD     UsageFault_Handler         ; Usage Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     DebugMon_Handler           ; Debug Monitor Handler
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; 外部中断表从异常号 16 开始。表项顺序必须与芯片参考手册完全一致；
                ; 不能按工程文件出现顺序调整，否则一个外设中断会跳到错误的处理函数。
                DCD     WWDG_IRQHandler                   ; Window WatchDog                                        
                DCD     PVD_IRQHandler                    ; PVD through EXTI Line detection                        
                DCD     TAMP_STAMP_IRQHandler             ; Tamper and TimeStamps through the EXTI line            
                DCD     RTC_WKUP_IRQHandler               ; RTC Wakeup through the EXTI line                       
                DCD     FLASH_IRQHandler                  ; FLASH                                           
                DCD     RCC_IRQHandler                    ; RCC                                             
                DCD     EXTI0_IRQHandler                  ; EXTI Line0                                             
                DCD     EXTI1_IRQHandler                  ; EXTI Line1                                             
                DCD     EXTI2_IRQHandler                  ; EXTI Line2                                             
                DCD     EXTI3_IRQHandler                  ; EXTI Line3                                             
                DCD     EXTI4_IRQHandler                  ; EXTI Line4                                             
                DCD     DMA1_Stream0_IRQHandler           ; DMA1 Stream 0                                   
                DCD     DMA1_Stream1_IRQHandler           ; DMA1 Stream 1                                   
                DCD     DMA1_Stream2_IRQHandler           ; DMA1 Stream 2                                   
                DCD     DMA1_Stream3_IRQHandler           ; DMA1 Stream 3                                   
                DCD     DMA1_Stream4_IRQHandler           ; DMA1 Stream 4                                   
                DCD     DMA1_Stream5_IRQHandler           ; DMA1 Stream 5                                   
                DCD     DMA1_Stream6_IRQHandler           ; DMA1 Stream 6                                   
                DCD     ADC_IRQHandler                    ; ADC1, ADC2 and ADC3s                            
                DCD     0                                 ; Reserved                                                
                DCD     0                                 ; Reserved                                               
                DCD     0                                 ; Reserved                                             
                DCD     0                                 ; Reserved                                               
                DCD     EXTI9_5_IRQHandler                ; External Line[9:5]s                                    
                DCD     TIM1_BRK_TIM9_IRQHandler          ; TIM1 Break and TIM9                   
                DCD     TIM1_UP_TIM10_IRQHandler          ; TIM1 Update and TIM10                 
                DCD     TIM1_TRG_COM_TIM11_IRQHandler     ; TIM1 Trigger and Commutation and TIM11
                DCD     TIM1_CC_IRQHandler                ; TIM1 Capture Compare                                   
                DCD     TIM2_IRQHandler                   ; TIM2                                            
                DCD     TIM3_IRQHandler                   ; TIM3                                            
                DCD     TIM4_IRQHandler                   ; TIM4                                            
                DCD     I2C1_EV_IRQHandler                ; I2C1 Event                                             
                DCD     I2C1_ER_IRQHandler                ; I2C1 Error                                             
                DCD     I2C2_EV_IRQHandler                ; I2C2 Event                                             
                DCD     I2C2_ER_IRQHandler                ; I2C2 Error                                               
                DCD     SPI1_IRQHandler                   ; SPI1                                            
                DCD     SPI2_IRQHandler                   ; SPI2                                            
                DCD     USART1_IRQHandler                 ; USART1                                          
                DCD     USART2_IRQHandler                 ; USART2                                          
                DCD     0                                 ; Reserved                                          
                DCD     EXTI15_10_IRQHandler              ; External Line[15:10]s                                  
                DCD     RTC_Alarm_IRQHandler              ; RTC Alarm (A and B) through EXTI Line                  
                DCD     OTG_FS_WKUP_IRQHandler            ; USB OTG FS Wakeup through EXTI line                        
                DCD     0                                 ; Reserved                  
                DCD     0                                 ; Reserved                 
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved                                   
                DCD     DMA1_Stream7_IRQHandler           ; DMA1 Stream7                                           
                DCD     0                                 ; Reserved                                             
                DCD     SDIO_IRQHandler                   ; SDIO                                            
                DCD     TIM5_IRQHandler                   ; TIM5                                            
                DCD     SPI3_IRQHandler                   ; SPI3                                            
                DCD     0                                 ; Reserved                                           
                DCD     0                                 ; Reserved                                           
                DCD     0                                 ; Reserved                   
                DCD     0                                 ; Reserved                   
                DCD     DMA2_Stream0_IRQHandler           ; DMA2 Stream 0                                   
                DCD     DMA2_Stream1_IRQHandler           ; DMA2 Stream 1                                   
                DCD     DMA2_Stream2_IRQHandler           ; DMA2 Stream 2                                   
                DCD     DMA2_Stream3_IRQHandler           ; DMA2 Stream 3                                   
                DCD     DMA2_Stream4_IRQHandler           ; DMA2 Stream 4
                DCD     0                                 ; Reserved  
                DCD     0                                 ; Reserved  
                DCD     0                                 ; Reserved                                              
                DCD     0                                 ; Reserved                                               
                DCD     0                                 ; Reserved                                               
                DCD     0                                 ; Reserved                                               
                DCD     OTG_FS_IRQHandler                 ; USB OTG FS                                      
                DCD     DMA2_Stream5_IRQHandler           ; DMA2 Stream 5                                   
                DCD     DMA2_Stream6_IRQHandler           ; DMA2 Stream 6                                   
                DCD     DMA2_Stream7_IRQHandler           ; DMA2 Stream 7                                   
                DCD     USART6_IRQHandler                 ; USART6                                           
                DCD     I2C3_EV_IRQHandler                ; I2C3 event                                             
                DCD     I2C3_ER_IRQHandler                ; I2C3 error                                             
                DCD     0                                 ; Reserved                     
                DCD     0                                 ; Reserved                       
                DCD     0                                 ; Reserved                         
                DCD     0                                 ; Reserved                                    
                DCD     0                                 ; Reserved  
                DCD     0                                 ; Reserved				                              
                DCD     0                                 ; Reserved
                DCD     FPU_IRQHandler                    ; FPU
                DCD     0                                 ; Reserved
		        DCD     0                                 ; Reserved
		        DCD     SPI4_IRQHandler                   ; SPI4
				DCD     SPI5_IRQHandler                   ; SPI5
                                         
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; 复位入口。该函数由向量表第 1 项提供给 Cortex-M4。
; 执行顺序：
;   ① LDR R0, =SystemInit：取得系统初始化函数地址；
;   ② BLX R0：调用 CMSIS/SystemInit，完成底层时钟、FPU 等早期配置；
;   ③ LDR R0, =__main：取得 ARM C 运行库入口地址；
;   ④ BX R0：跳转到 C 运行库，由运行库完成数据段搬运、清零 BSS、
;      堆栈环境准备，最终调用用户的 main()。
; 这里没有直接调用 main()，因为 C 全局变量初始化等工作必须先由 __main 完成。
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  SystemInit
        IMPORT  __main

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; 默认异常处理函数。
; 这些函数以 [WEAK] 导出：如果其他 C 文件提供同名的强符号实现，链接器会优先
; 使用用户实现；如果没有实现，则向量表仍能解析到这里，并在 B . 处永久停留。
; 永久停留便于调试器捕获异常，避免错误处理函数返回后继续执行未知地址。

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

; 外部中断的弱符号声明。下面的 EXPORT 只声明符号可被链接器看到，真正的默认
; 函数体由同一 PROC 中的同名标签共享；因此未实现的 IRQ 会统一进入 B .。
Default_Handler PROC

                EXPORT  WWDG_IRQHandler                   [WEAK]                                        
                EXPORT  PVD_IRQHandler                    [WEAK]                      
                EXPORT  TAMP_STAMP_IRQHandler             [WEAK]         
                EXPORT  RTC_WKUP_IRQHandler               [WEAK]                     
                EXPORT  FLASH_IRQHandler                  [WEAK]                                         
                EXPORT  RCC_IRQHandler                    [WEAK]                                            
                EXPORT  EXTI0_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI1_IRQHandler                  [WEAK]                                             
                EXPORT  EXTI2_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI3_IRQHandler                  [WEAK]                                           
                EXPORT  EXTI4_IRQHandler                  [WEAK]                                            
                EXPORT  DMA1_Stream0_IRQHandler           [WEAK]                                
                EXPORT  DMA1_Stream1_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream2_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream3_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream4_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream5_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream6_IRQHandler           [WEAK]                                   
                EXPORT  ADC_IRQHandler                    [WEAK]                                                                        
                EXPORT  EXTI9_5_IRQHandler                [WEAK]                                    
                EXPORT  TIM1_BRK_TIM9_IRQHandler          [WEAK]                  
                EXPORT  TIM1_UP_TIM10_IRQHandler          [WEAK]                
                EXPORT  TIM1_TRG_COM_TIM11_IRQHandler     [WEAK] 
                EXPORT  TIM1_CC_IRQHandler                [WEAK]                                   
                EXPORT  TIM2_IRQHandler                   [WEAK]                                            
                EXPORT  TIM3_IRQHandler                   [WEAK]                                            
                EXPORT  TIM4_IRQHandler                   [WEAK]                                            
                EXPORT  I2C1_EV_IRQHandler                [WEAK]                                             
                EXPORT  I2C1_ER_IRQHandler                [WEAK]                                             
                EXPORT  I2C2_EV_IRQHandler                [WEAK]                                            
                EXPORT  I2C2_ER_IRQHandler                [WEAK]                                               
                EXPORT  SPI1_IRQHandler                   [WEAK]                                           
                EXPORT  SPI2_IRQHandler                   [WEAK]                                            
                EXPORT  USART1_IRQHandler                 [WEAK]                                          
                EXPORT  USART2_IRQHandler                 [WEAK]                                                                                  
                EXPORT  EXTI15_10_IRQHandler              [WEAK]                                  
                EXPORT  RTC_Alarm_IRQHandler              [WEAK]                  
                EXPORT  OTG_FS_WKUP_IRQHandler            [WEAK]                        
                EXPORT  DMA1_Stream7_IRQHandler           [WEAK]                                                                                     
                EXPORT  SDIO_IRQHandler                   [WEAK]                                             
                EXPORT  TIM5_IRQHandler                   [WEAK]                                             
                EXPORT  SPI3_IRQHandler                   [WEAK]                                                               
                EXPORT  DMA2_Stream0_IRQHandler           [WEAK]                                  
                EXPORT  DMA2_Stream1_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream2_IRQHandler           [WEAK]                                    
                EXPORT  DMA2_Stream3_IRQHandler           [WEAK]                                    
                EXPORT  DMA2_Stream4_IRQHandler           [WEAK]                                                                                                     
                EXPORT  OTG_FS_IRQHandler                 [WEAK]                                       
                EXPORT  DMA2_Stream5_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream6_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream7_IRQHandler           [WEAK]                                   
                EXPORT  USART6_IRQHandler                 [WEAK]                                           
                EXPORT  I2C3_EV_IRQHandler                [WEAK]                                              
                EXPORT  I2C3_ER_IRQHandler                [WEAK]                                              
                EXPORT  FPU_IRQHandler                    [WEAK]
				EXPORT  SPI4_IRQHandler                   [WEAK]
                EXPORT  SPI5_IRQHandler                   [WEAK]

WWDG_IRQHandler                                                       
PVD_IRQHandler                                      
TAMP_STAMP_IRQHandler                  
RTC_WKUP_IRQHandler                                
FLASH_IRQHandler                                                       
RCC_IRQHandler                                                            
EXTI0_IRQHandler                                                          
EXTI1_IRQHandler                                                           
EXTI2_IRQHandler                                                          
EXTI3_IRQHandler                                                         
EXTI4_IRQHandler                                                          
DMA1_Stream0_IRQHandler                                       
DMA1_Stream1_IRQHandler                                          
DMA1_Stream2_IRQHandler                                          
DMA1_Stream3_IRQHandler                                          
DMA1_Stream4_IRQHandler                                          
DMA1_Stream5_IRQHandler                                          
DMA1_Stream6_IRQHandler                                          
ADC_IRQHandler                                                                                                    
EXTI9_5_IRQHandler                                                
TIM1_BRK_TIM9_IRQHandler                        
TIM1_UP_TIM10_IRQHandler                      
TIM1_TRG_COM_TIM11_IRQHandler  
TIM1_CC_IRQHandler                                               
TIM2_IRQHandler                                                           
TIM3_IRQHandler                                                           
TIM4_IRQHandler                                                           
I2C1_EV_IRQHandler                                                         
I2C1_ER_IRQHandler                                                         
I2C2_EV_IRQHandler                                                        
I2C2_ER_IRQHandler                                                           
SPI1_IRQHandler                                                          
SPI2_IRQHandler                                                           
USART1_IRQHandler                                                       
USART2_IRQHandler                                                                                                           
EXTI15_10_IRQHandler                                            
RTC_Alarm_IRQHandler                            
OTG_FS_WKUP_IRQHandler                                                                           
DMA1_Stream7_IRQHandler                                                                                                             
SDIO_IRQHandler                                                            
TIM5_IRQHandler                                                            
SPI3_IRQHandler                                                                                     
DMA2_Stream0_IRQHandler                                         
DMA2_Stream1_IRQHandler                                          
DMA2_Stream2_IRQHandler                                           
DMA2_Stream3_IRQHandler                                           
DMA2_Stream4_IRQHandler                                                                                                                                  
OTG_FS_IRQHandler                                                    
DMA2_Stream5_IRQHandler                                          
DMA2_Stream6_IRQHandler                                          
DMA2_Stream7_IRQHandler                                          
USART6_IRQHandler                                                        
I2C3_EV_IRQHandler                                                          
I2C3_ER_IRQHandler                                                          
FPU_IRQHandler
SPI4_IRQHandler
SPI5_IRQHandler

                ; 所有未被用户覆盖的外部 IRQ 都汇聚到这里，进入不可返回的故障现场。
                B       .

                ENDP

                ALIGN

;*******************************************************************************
; 用户栈和堆初始化接口
;*******************************************************************************
; ARM C 运行库根据工程是否启用 MicroLIB 选择不同接口：
; - MicroLIB：直接使用 __initial_sp、__heap_base、__heap_limit；
; - 标准库：调用 __user_initial_stackheap，按 R0~R3 返回四个边界。
; 返回约定（非 MicroLIB）：
;   R0 = Heap_Mem                  堆起始地址
;   R1 = Stack_Mem + Stack_Size    栈顶地址
;   R2 = Heap_Mem + Heap_Size      堆结束地址
;   R3 = Stack_Mem                 栈底地址
; 这些地址由链接器符号解析，不能把它们误解为实际的 C 变量内容。
                 IF      :DEF:__MICROLIB
                
                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                
                 ELSE
                
                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                 LDR     R0, =  Heap_Mem              ; 返回堆起始地址。
                 LDR     R1, =(Stack_Mem + Stack_Size) ; 返回栈顶，即初始 MSP。
                 LDR     R2, = (Heap_Mem +  Heap_Size) ; 返回堆结束地址。
                 LDR     R3, = Stack_Mem              ; 返回栈底地址。
                 BX      LR                           ; 返回 C 运行库调用点。

                 ALIGN

                 ENDIF

                 END
