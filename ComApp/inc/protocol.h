#ifndef PROTOCOL_H
#define PROTOCOL_H

/*解析蓝牙数据包，发出CAN数据帧*/

#include "BlueTooth.h"
#include "fdcan.h"
#include "FD_Canqueue.h"

extern FDCAN_SendQueueType Chassis_queue;
extern FDCAN_SendQueueType BigBlock_queue;
extern FDCAN_SendQueueType Sky_queue;
extern FDCAN_SendQueueType Ball_queue;

/*
 * 按钮状态类型
 * pre : 上一次状态（用于边沿检测）
 * now : 当前状态
 * 上升沿(按下瞬间) = now && !pre
 * 下降沿(松开瞬间) = !now && pre
 * 适用：所有按钮型变量（protect、复位、预取、取块、放块等）
 */
typedef struct {
    uint8_t pre;    // 上一次状态
    uint8_t now;    // 当前状态
} Btn_t;


typedef struct 
{
        /* ==================== 底盘机构 ==================== */
    uint8_t  Chassis_Enable;        // bools[0]  底盘使能(拨杆/开关)
    Btn_t    Chassis_Protect;       // bools[1]  底盘保护(按钮)
    Btn_t    Chassis_Reset;         // bools[2]  底盘复位(按钮)
    uint8_t  Chassis_LockAngle;     // bools[3]  底盘锁角(开关)

        /* ==================== 底盘速度(short / 摇杆) ==================== */
    short CarVx;                    // shorts[0] vx(摇杆)
    short CarVy;                    // shorts[1] vy(摇杆)
    short CarVw;                    // shorts[2] w(摇杆)

        /* ==================== 角度(float / 可编辑文本框) ==================== */
    float Angle;                    // floats[0] 角度(可编辑文本框)

}Chassis_CtrlWord_t;

typedef struct 
{
    /* ==================== 大地块机构 ==================== */
    uint8_t  BigBlock_Enable;       // bools[4]  大地块使能(开关)
    Btn_t    BigBlock_Protect;      // bools[5]  大地块保护(按钮)
    Btn_t    BigBlock_Reset;        // bools[6]  大地块复位(按钮)
    Btn_t    BigBlock_PreGrab;      // bools[7]  预取块(按钮)
    Btn_t    BigBlock_Grab;         // bools[8]  取块(按钮)
    Btn_t    BigBlock_Put;          // bools[9]  放块(按钮)
    uint8_t  BigBlock_LayerHeight;  // bools[10] 层高(拨杆/开关)

}BigBlock_CtrlWord_t;

typedef struct 
{
        /* ==================== 天空块机构 ==================== */
    uint8_t  SkyBlock_Enable;       // bools[11] 天空块使能(开关)
    Btn_t    SkyBlock_Protect;      // bools[12] 天空块保护(按钮)
    Btn_t    SkyBlock_Reset;        // bools[13] 天空块复位(按钮)
    Btn_t    SkyBlock_Grab;         // bools[14] 取天空块(按钮)
    Btn_t    SkyBlock_Put;          // bools[15] 放天空块(按钮)
    Btn_t    SkyBlock_Carry;        // bools[16] 持天空块(按钮)/放球位置
    uint8_t  SkyBlock_BlockOrBall;  // bools[17] 块/球选择(开关)
    uint8_t  SkyBlock_JawOpenClose; // bools[18] 爪子开/关(开关)

}Sky_CtrlWord_t;

typedef struct 
{
    /* ==================== 灵石(球)机构 ==================== */
    uint8_t  Ball_Enable;           // bools[19] 灵石使能(开关)
    Btn_t    Ball_Protect;          // bools[20] 灵石保护(按钮)
    Btn_t    Ball_Reset;            // bools[21] 灵石复位(按钮)
    Btn_t    Ball_Grab;             // bools[22] 取球(按钮)
    Btn_t    Ball_Carry;            // bools[23] 持球(按钮)
    Btn_t    Ball_Put;              // bools[24] 放球(按钮)
    uint8_t  Ball_AirPump;          // bools[25] 气泵开/关(开关)

}Ball_CtrlWord_t;


/* 全局控制字变量 */
extern Chassis_CtrlWord_t Chassis_CtrlWord;
extern BigBlock_CtrlWord_t BigBlock_CtrlWord;
extern Sky_CtrlWord_t Sky_CtrlWord;
extern Ball_CtrlWord_t Ball_CtrlWord;


void Chassis_CtrlWord_SendCAN(void);
void BigBlock_CtrlWord_SendCAN(void);
void Sky_CtrlWord_t_SendCAN(void);
void Ball_CtrlWord_t_SendCAN(void);


/*解析蓝牙数据包*/
void Car_CtrlWord_Unpack(void);

#endif
