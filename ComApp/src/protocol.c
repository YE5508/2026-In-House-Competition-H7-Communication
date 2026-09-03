#include "protocol.h"
#include "protocolcanid.h"
#include "FD_CanQueue.h"
#include <string.h>

FDCAN_SendQueueType Chassis_queue;
FDCAN_SendQueueType BigBlock_queue;
FDCAN_SendQueueType Sky_queue;
FDCAN_SendQueueType Ball_queue;

Chassis_CtrlWord_t Chassis_CtrlWord;
BigBlock_CtrlWord_t BigBlock_CtrlWord;
Sky_CtrlWord_t Sky_CtrlWord;
Ball_CtrlWord_t Ball_CtrlWord;
/*
 * 函数名：Car_CtrlWord_Unpack
 * 功能：  从全局接收缓冲区 RxMsgPack 中解包数据，
 *         更新 Car_CtrlWord 全局变量
 *         所有按钮型变量自动维护 pre(上一次) / now(当前)，供边沿检测
 *         开关型变量直接赋值
 * 参数：  无
 * 返回：  无
 */
void Car_CtrlWord_Unpack(void)
{
    /* ========== 底盘机构 ========== */
    Chassis_CtrlWord.Chassis_Enable    = RxMsgPack.bools[0];
    Chassis_CtrlWord.Chassis_Protect.pre = Chassis_CtrlWord.Chassis_Protect.now;
    Chassis_CtrlWord.Chassis_Protect.now = RxMsgPack.bools[1];
    Chassis_CtrlWord.Chassis_Reset.pre   = Chassis_CtrlWord.Chassis_Reset.now;
    Chassis_CtrlWord.Chassis_Reset.now   = RxMsgPack.bools[2];
    Chassis_CtrlWord.Chassis_LockAngle = RxMsgPack.bools[3];

        /* ========== 底盘速度(short) ========== */
    Chassis_CtrlWord.CarVx = RxMsgPack.shorts[0];
    Chassis_CtrlWord.CarVy = -RxMsgPack.shorts[1];
    Chassis_CtrlWord.CarVw = -RxMsgPack.shorts[2];

    /* ========== 角度(float) ========== */
    Chassis_CtrlWord.Angle = RxMsgPack.floats[0];

    /* ========== 大地块机构 ========== */
    BigBlock_CtrlWord.BigBlock_Enable     = RxMsgPack.bools[4];
    BigBlock_CtrlWord.BigBlock_Protect.pre = BigBlock_CtrlWord.BigBlock_Protect.now;
    BigBlock_CtrlWord.BigBlock_Protect.now = RxMsgPack.bools[5];
    BigBlock_CtrlWord.BigBlock_Reset.pre   = BigBlock_CtrlWord.BigBlock_Reset.now;
    BigBlock_CtrlWord.BigBlock_Reset.now   = RxMsgPack.bools[6];
    BigBlock_CtrlWord.BigBlock_PreGrab.pre = BigBlock_CtrlWord.BigBlock_PreGrab.now;
    BigBlock_CtrlWord.BigBlock_PreGrab.now = RxMsgPack.bools[7];
    BigBlock_CtrlWord.BigBlock_Grab.pre    = BigBlock_CtrlWord.BigBlock_Grab.now;
    BigBlock_CtrlWord.BigBlock_Grab.now    = RxMsgPack.bools[8];
    BigBlock_CtrlWord.BigBlock_Put.pre     = BigBlock_CtrlWord.BigBlock_Put.now;
    BigBlock_CtrlWord.BigBlock_Put.now     = RxMsgPack.bools[9];
    BigBlock_CtrlWord.BigBlock_LayerHeight = RxMsgPack.bools[10];
    BigBlock_CtrlWord.BigBlock_Release1.pre=BigBlock_CtrlWord.BigBlock_Release1.now;
    BigBlock_CtrlWord.BigBlock_Release1.now=RxMsgPack.bools[28];

    /* ========== 天空块机构 ========== */
    Sky_CtrlWord.SkyBlock_Enable      = RxMsgPack.bools[11];
    Sky_CtrlWord.SkyBlock_Protect.pre = Sky_CtrlWord.SkyBlock_Protect.now;
    Sky_CtrlWord.SkyBlock_Protect.now = RxMsgPack.bools[12];
    Sky_CtrlWord.SkyBlock_Reset.pre   = Sky_CtrlWord.SkyBlock_Reset.now;
    Sky_CtrlWord.SkyBlock_Reset.now   = RxMsgPack.bools[13];
    Sky_CtrlWord.SkyBlock_Grab.pre    = Sky_CtrlWord.SkyBlock_Grab.now;
    Sky_CtrlWord.SkyBlock_Grab.now    = RxMsgPack.bools[14];
    Sky_CtrlWord.SkyBlock_Put.pre     = Sky_CtrlWord.SkyBlock_Put.now;
    Sky_CtrlWord.SkyBlock_Put.now     = RxMsgPack.bools[15];
    Sky_CtrlWord.SkyBlock_Carry.pre   = Sky_CtrlWord.SkyBlock_Carry.now;
    Sky_CtrlWord.SkyBlock_Carry.now   = RxMsgPack.bools[16];
    Sky_CtrlWord.SkyBlock_BlockOrBall  = RxMsgPack.bools[17];
    Sky_CtrlWord.SkyBlock_JawOpenClose = RxMsgPack.bools[18];
    Sky_CtrlWord.SkyBlock_Idle.pre=Sky_CtrlWord.SkyBlock_Idle.now;
    Sky_CtrlWord.SkyBlock_Idle.now=RxMsgPack.bools[27];

    /* ========== 灵石(球)机构 ========== */
    Ball_CtrlWord.Ball_Enable       = RxMsgPack.bools[19];
    Ball_CtrlWord.Ball_Protect.pre  = Ball_CtrlWord.Ball_Protect.now;
    Ball_CtrlWord.Ball_Protect.now  = RxMsgPack.bools[20];
    Ball_CtrlWord.Ball_Reset.pre    = Ball_CtrlWord.Ball_Reset.now;
    Ball_CtrlWord.Ball_Reset.now    = RxMsgPack.bools[21];
    Ball_CtrlWord.Ball_Grab.pre     = Ball_CtrlWord.Ball_Grab.now;
    Ball_CtrlWord.Ball_Grab.now     = RxMsgPack.bools[22];
    Ball_CtrlWord.Ball_Carry.pre    = Ball_CtrlWord.Ball_Carry.now;
    Ball_CtrlWord.Ball_Carry.now    = RxMsgPack.bools[23];
    Ball_CtrlWord.Ball_Put.pre      = Ball_CtrlWord.Ball_Put.now;
    Ball_CtrlWord.Ball_Put.now      = RxMsgPack.bools[24];
    Ball_CtrlWord.Ball_AirPump      = RxMsgPack.bools[25];
    Ball_CtrlWord.Ball_BallorBlock  = RxMsgPack.bools[26];

}


/* 入队一帧扩展数据帧；实际发送由 CAN 队列定时出队完成。 */
static bool Protocol_Enqueue(FDCAN_SendQueueType* queue,uint32_t id, uint8_t dlc, const uint8_t *data)
{
    FDCAN_RxHeaderTypeDef header;

    HeaderPrepare(id, dlc, &header);
    return CAN_Enqueue(queue, header, (uint8_t *)data);
}

static bool Protocol_EnqueueCommand(FDCAN_SendQueueType* queue,uint32_t id, uint8_t command, uint8_t value)
{
    const uint8_t data[2] = {command, value};
    return Protocol_Enqueue(queue,id, 2U, data);
}


/*将解包后的蓝牙控制命令按照不同的机构分别进CAN发送队列*/
void Chassis_CtrlWord_SendCAN(void)
{
    static bool lock_angle_valid = false;
    static uint8_t last_lock_angle_state = 0U;

    uint8_t data[6];
    int16_t speed;

    if(Chassis_CtrlWord.Chassis_Enable&&Chassis_CtrlWord.Chassis_Protect.now&&!Chassis_CtrlWord.Chassis_Protect.pre)
    {
        Protocol_EnqueueCommand(&Chassis_queue,CHASSIS_ENABLE, 'M', Chassis_CtrlWord.Chassis_Enable);
    }
    else if(!Chassis_CtrlWord.Chassis_Enable)
    {
        Protocol_EnqueueCommand(&Chassis_queue,CHASSIS_ENABLE, 'M', Chassis_CtrlWord.Chassis_Enable);
    }

        /* short 已由蓝牙协议按目标比例提供，直接以小端格式拷贝。 */
    speed = Chassis_CtrlWord.CarVx; memcpy(&data[0], &speed, sizeof(speed));
    speed = Chassis_CtrlWord.CarVy; memcpy(&data[2], &speed, sizeof(speed));
    speed = Chassis_CtrlWord.CarVw; memcpy(&data[4], &speed, sizeof(speed));
    Protocol_Enqueue(&Chassis_queue,CHASSIS_SPEED_CONTROL, 6U, data);

    if(Chassis_CtrlWord.Chassis_Reset.now&&!Chassis_CtrlWord.Chassis_Reset.pre)
    {
        Protocol_EnqueueCommand(&Chassis_queue,CHASSIS_RESET, 'R', 'S');
    }

    if(Chassis_CtrlWord.Chassis_LockAngle)
    {
        memcpy(data, &Chassis_CtrlWord.Angle, sizeof(Chassis_CtrlWord.Angle));
        Protocol_Enqueue(&Chassis_queue,CHASSIS_LOCK_ANGLE, 4U, data);
    }


 

    

}

void BigBlock_CtrlWord_SendCAN(void)
{
    if(BigBlock_CtrlWord.BigBlock_Enable&&BigBlock_CtrlWord.BigBlock_Protect.now&&!BigBlock_CtrlWord.BigBlock_Protect.pre)
    {
        Protocol_EnqueueCommand(&BigBlock_queue,BIGBLOCK_ENABLE, 'M', BigBlock_CtrlWord.BigBlock_Enable);
    }
    else if(BigBlock_CtrlWord.BigBlock_Enable==0) 
    {
        Protocol_EnqueueCommand(&BigBlock_queue,BIGBLOCK_ENABLE, 'M', BigBlock_CtrlWord.BigBlock_Enable);   
    }

    if(BigBlock_CtrlWord.BigBlock_PreGrab.now&&!BigBlock_CtrlWord.BigBlock_PreGrab.pre)
    {
        Protocol_EnqueueCommand(&BigBlock_queue,BIGBLOCK_READY_TO_GRAB, 'P', 'E');
    }

    if(BigBlock_CtrlWord.BigBlock_Reset.now&&!BigBlock_CtrlWord.BigBlock_Reset.pre)
    {
        Protocol_EnqueueCommand(&BigBlock_queue,BIGBLOCK_RESET, 'R', 'S');
    }

    if(BigBlock_CtrlWord.BigBlock_Grab.now&&!BigBlock_CtrlWord.BigBlock_Grab.pre)
    {
        Protocol_EnqueueCommand(&BigBlock_queue,BIGBLOCK_GRAB, 'G', BigBlock_CtrlWord.BigBlock_LayerHeight+1);
    }

    if(BigBlock_CtrlWord.BigBlock_Put.now&&!BigBlock_CtrlWord.BigBlock_Put.pre)
    {
        Protocol_EnqueueCommand(&BigBlock_queue,BIGBLOCK_PUT, 'P', BigBlock_CtrlWord.BigBlock_LayerHeight+1);
    }
    if(BigBlock_CtrlWord.BigBlock_Release1.now&&!BigBlock_CtrlWord.BigBlock_Release1.pre)
    {
        Protocol_EnqueueCommand(&BigBlock_queue,BIGBLOCK_RELEASE1,'S',BigBlock_CtrlWord.BigBlock_Release1.now);
    }

}

void Sky_CtrlWord_t_SendCAN(void)
{
    static bool jaw_state_valid = false;
    static uint8_t last_jaw_state = 0U;

    if(Sky_CtrlWord.SkyBlock_Enable&&Sky_CtrlWord.SkyBlock_Protect.now&&!Sky_CtrlWord.SkyBlock_Protect.pre)
    {
        Protocol_EnqueueCommand(&Sky_queue,SKYBLOCK_ENABLE, 'M', Sky_CtrlWord.SkyBlock_Enable);
    }
    else if(!Sky_CtrlWord.SkyBlock_Enable)
    {
        Protocol_EnqueueCommand(&Sky_queue,SKYBLOCK_ENABLE, 'M', 0);
    }

    if(Sky_CtrlWord.SkyBlock_Reset.now&&!Sky_CtrlWord.SkyBlock_Reset.pre)
    {
        Protocol_EnqueueCommand(&Sky_queue,SKYBLOCK_RESET, 'R', 'S');
    }

    if(Sky_CtrlWord.SkyBlock_Grab.now&&!Sky_CtrlWord.SkyBlock_Grab.pre)
    {
        if(!Sky_CtrlWord.SkyBlock_BlockOrBall)
        {
            Protocol_EnqueueCommand(&Sky_queue,SKYBLOCK_GRAB_BLOCK, 'G', 'S');
        }

        else
        {
            Protocol_EnqueueCommand(&Sky_queue,SKYBLOCK_GRAB_BALL, 'G', 'B');
        }
    }

    if(Sky_CtrlWord.SkyBlock_Put.now&&!Sky_CtrlWord.SkyBlock_Put.pre)
    {
        Protocol_EnqueueCommand(&Sky_queue,SKYBLOCK_PUT, 'P', 'S');
    }

    if(Sky_CtrlWord.SkyBlock_Carry.now&&!Sky_CtrlWord.SkyBlock_Carry.pre)
    {
        Protocol_EnqueueCommand(&Sky_queue,SKYBLOCK_ARM_RESET, 'A', 'R');
    }

    if (!jaw_state_valid || Sky_CtrlWord.SkyBlock_JawOpenClose != last_jaw_state)
    {
        if (Protocol_EnqueueCommand(&Sky_queue, SKYBLOCK_JAW_OPEN, 'R', Sky_CtrlWord.SkyBlock_JawOpenClose))
        {
            last_jaw_state = Sky_CtrlWord.SkyBlock_JawOpenClose;
            jaw_state_valid = true;
        }
    }
    if(Sky_CtrlWord.SkyBlock_Idle.now&&!Sky_CtrlWord.SkyBlock_Idle.pre)
    {
        Protocol_EnqueueCommand(&Sky_queue,SKYBLCOK_IDLE, 'I', 'M');
    }
    
    
}

void Ball_CtrlWord_t_SendCAN(void)
{
    static bool air_pump_state_valid = false;
    static uint8_t last_air_pump_state = 0U;
    static bool ballorblock_state_valid=false;
    static uint8_t last_ballorblock_state =0U;


    if(Ball_CtrlWord.Ball_Enable&&Ball_CtrlWord.Ball_Protect.now&&!Ball_CtrlWord.Ball_Protect.pre)
    {
        Protocol_EnqueueCommand(&Ball_queue,BALL_ENABLE, 'M', Ball_CtrlWord.Ball_Enable);
    }
    else if(!Ball_CtrlWord.Ball_Enable)
    {
        Protocol_EnqueueCommand(&Ball_queue,BALL_ENABLE, 'M', Ball_CtrlWord.Ball_Enable);
    }

    if(Ball_CtrlWord.Ball_Reset.now&&!Ball_CtrlWord.Ball_Reset.pre)
    {
        Protocol_EnqueueCommand(&Ball_queue,BALL_RESET, 'R', 'S');
    }

    if(Ball_CtrlWord.Ball_Grab.now&&!Ball_CtrlWord.Ball_Grab.pre)
    {
        Protocol_EnqueueCommand(&Ball_queue,BALL_GRAB, 'R', 'S');
    }

    if(Ball_CtrlWord.Ball_Carry.now&&!Ball_CtrlWord.Ball_Carry.pre)
    {
        Protocol_EnqueueCommand(&Ball_queue,BALL_CARRY, 'P', 'B');
    }

    if(Ball_CtrlWord.Ball_Put.now&&!Ball_CtrlWord.Ball_Put.pre)
    {
        Protocol_EnqueueCommand(&Ball_queue,BALL_PUT, 'R', 'B');
    }

    if (!air_pump_state_valid || Ball_CtrlWord.Ball_AirPump != last_air_pump_state)
    {
        if (Protocol_EnqueueCommand(&Ball_queue, BALL_OPEN, 'R', Ball_CtrlWord.Ball_AirPump))
        {
            last_air_pump_state = Ball_CtrlWord.Ball_AirPump;
            air_pump_state_valid = true;
        }
    }
    if(!ballorblock_state_valid|| Ball_CtrlWord.Ball_BallorBlock!=last_ballorblock_state)
    {
        if(Protocol_EnqueueCommand(&Ball_queue,BALL_BALLORBLOCK,'S',Ball_CtrlWord.Ball_BallorBlock))
        {
            last_ballorblock_state = Ball_CtrlWord.Ball_BallorBlock;
            ballorblock_state_valid = true;
        }
    }
}
