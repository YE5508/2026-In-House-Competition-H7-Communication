# 武汉大学 Robocon 2026 校内赛 A2 组主控

> STM32H723 主控工程 | 上位机、主控与底盘/机构从板通信 | FreeRTOS + FDCAN

这是武汉大学 Robocon 2026 校内赛 A2 组的主控固件。工程以 STM32H723ZETx 为核心，负责接收上位机操作指令、解析 A2 通信协议、向底盘与三类机构下发 CAN 控制命令，并将底盘状态回传给上位机。

项目的核心并不是单一外设驱动，而是把“上位机意图”稳定地转换为多节点、可追踪的执行命令：协议字段解包、按钮边沿检测、机构命令路由、CAN 队列发送和反馈回传形成了一条完整链路。

## 项目概览

| 项目 | 内容 |
| --- | --- |
| 比赛 | 武汉大学 Robocon 2026 校内赛 |
| 组别 | A2 组 |
| MCU | STM32H723ZETx，Cortex-M7 |
| 系统 | FreeRTOS（CMSIS-OS2） |
| 通信 | UART/蓝牙调试链路 + 1 Mbps 扩展帧 FDCAN |
| 开发工具 | STM32CubeMX、Keil MDK-ARM |
| 主要语言 | C |

## 通信架构

```text
上位机
  │  UART/蓝牙，115200 8N1
  ▼
主控 STM32H723
  ├─ ComApp：解包、控制字维护、命令编码
  ├─ Bluetooth：帧头/帧尾/校验和与 DMA 接收
  └─ FDCAN：按机构分队列，统一路由与发送
       ├─ 底盘
       ├─ 大地块机构
       ├─ 天空块机构
       └─ 灵石机构
```

### A2 协议要点

- 蓝牙帧格式：`0xA5 + Payload + 低 8 位累加和 + 0x5A`。
- Payload 采用固定顺序：bool（按位打包）→ byte → short → int → float；多字节数据使用小端序。
- 当前主控接收配置为 29 个 bool、3 个 short、1 个 float，共 14 字节原始数据（bool 占 4 字节）；发送反馈为 1 个 float（底盘 Yaw）。字段数量由 `Communication/Bluetooth/bluetooth.h` 的宏集中配置。
- CAN 使用 29 位扩展数据帧，默认速率 1 Mbps。ID 按“发送设备 / CAN 路由 / 接收设备 / 操作码”组织，例如 `0x01010202` 表示主控通过 CAN2 向底盘发送速度控制。
- 机构命令分为底盘、大地块、天空块、灵石四组，分别进入独立发送队列，降低业务模块之间的耦合。

协议字段、CAN ID 和数据字节定义可参考 [`通信协议.md`](通信协议.md)；当前固件实际字段数量以 `Communication/Bluetooth/bluetooth.h` 与 `ComApp/` 实现为准。

## ComApp：个人工作重点

`ComApp/` 是本工程中最具业务价值的协议适配层，主要实现如下职责：

1. **控制包解包**：将 `RxMsgPack` 中的原始字段映射为底盘、大地块、天空块和灵石四类控制字结构体。
2. **按钮边沿检测**：每个按钮同时保留 `pre/now` 状态，将持续输入转换为一次性动作事件，避免复位、抓取、放置等命令在每个周期重复发送。
3. **机构命令编码**：根据使能状态、动作边沿、层高和球/块模式，生成对应 CAN ID 与 2～6 字节数据。
4. **状态变化发送**：夹爪、气泵和球/块模式等开关仅在首次运行或状态变化时入队，减少总线上的无效流量。
5. **发送解耦**：`Protocol_Enqueue()` 只负责封装并入队，实际发送由 `CAN_DequeueTx()` 在任务/定时调度中完成，业务逻辑不直接阻塞在硬件发送上。

关键入口：

```c
Car_CtrlWord_Unpack();          // 原始 Payload -> 四类控制字
Chassis_CtrlWord_SendCAN();     // 底盘命令
BigBlock_CtrlWord_SendCAN();    // 大地块命令
Sky_CtrlWord_t_SendCAN();       // 天空块命令
Ball_CtrlWord_t_SendCAN();      // 灵石命令
```

对应实现位于 [`ComApp/src/protocol.c`](ComApp/src/protocol.c)，CAN ID 集中定义在 [`ComApp/inc/protocolcanid.h`](ComApp/inc/protocolcanid.h)。

## 数据流与调度

```text
UART 空闲中断 / DMA
  → Deal_RxPack()
  → 校验和与长度检查
  → Debug_Receive()
  → Car_CtrlWord_Unpack()
  → 各机构 CtrlWord_SendCAN()
  → 机构 CAN 队列
  → CAN_DequeueTx()
  → 底盘 / 机构从板
```

- UART 接收采用 Receive-To-Idle DMA，接收缓冲区放在 H7 DMA 可访问的 D2 SRAM。
- 接收包先完成包头、包尾、长度和累加和检查，再进入业务层，避免半包或脏数据触发机构动作。
- 发送队列按机构拆分：`Chassis_queue`、`BigBlock_queue`、`Sky_queue`、`Ball_queue`。
- `Application/src/myostasks.c` 中的控制任务每 10 ms 负责对应队列出队；CAN 接收中断负责解析底盘 Yaw 反馈，并由 `Yaw_Feedback_Task` 周期性回传上位机。
- 电机驱动和用户通信共用 FDCAN 外设，但在队列和 ID 层面隔离，便于后续增加机构或替换执行器。

## 工程结构

```text
ComApp/          A2 协议适配：控制字、CAN ID、命令编码
Communication/   蓝牙帧处理、VOFA+ 调试输出
Application/     FreeRTOS 任务、中断回调、业务入口
Motor/           DJI、VESC、ZDrive、Unitree 及 CAN 队列
BSP/             LED、蜂鸣器、板级初始化
Algorithm/       PID、滤波、环形缓冲区、数学工具
Config/          板级、应用和电机配置
Core/            CubeMX 生成的 HAL、FDCAN、UART、定时器代码
DOCS/            架构、硬件、移植和调试文档
MDK-ARM/         Keil 工程、启动文件和链接脚本
```

## 构建与运行

### Keil

1. 使用 Keil MDK-ARM 打开 [`MDK-ARM/27RC_Proj_Template.uvprojx`](MDK-ARM/27RC_Proj_Template.uvprojx)。
2. 检查 `Config/board_config.h`、`Config/app_config.h` 和 `Config/motor_config.h` 中的板卡、通信和电机开关。
3. 编译并下载到 STM32H723ZETx。

### 命令行（可选）

```powershell
powershell -File Tools\build.ps1
# 全量构建
powershell -File Tools\build.ps1 -Rebuild
```

构建日志位于 `Tools/logs/`。首次移植或重新生成 CubeMX 工程时，请同时阅读 [`DOCS/porting_guide.md`](DOCS/porting_guide.md)。

## 调试与验证

- 使用 CAN 分析仪检查扩展帧 ID、DLC 和数据端序，重点验证 `ComApp/inc/protocolcanid.h` 中的机构命令。
- 使用蓝牙/串口工具按 `通信协议.md` 组包，验证校验失败、长度异常和按钮上升沿行为。
- 使用 VOFA+ 观察 Yaw、目标速度和电机状态；VOFA+ 配置见 [`DOCS/vofa_plus.md`](DOCS/vofa_plus.md)。
- 调试时优先确认链路层数据，再确认控制字映射，最后确认机构从板执行状态，便于快速定位问题所在层级。

## 当前边界

- 底盘位置模式 `0x01010203` 暂未下发。
- CAN 发送队列尚未加入跨上下文并发保护，队列容量和发送周期需要结合实车负载校准。
- 蓝牙协议当前使用累加和校验，不提供自动重发；从板错误码的具体语义仍由从板协议约定。
- CAN 反馈字段目前以底盘 Yaw 为主，更多状态反馈需要在联调中继续扩展。

## 相关文档

- [`通信协议.md`](通信协议.md)：上位机、主控、从板之间的字段和报文定义
- [`DOCS/architecture.md`](DOCS/architecture.md)：中断、任务、电机时序和内存布局
- [`DOCS/hardware_notes.md`](DOCS/hardware_notes.md)：板载资源与引脚速查
- [`DOCS/coding_standard.md`](DOCS/coding_standard.md)：C 语言编码约定
