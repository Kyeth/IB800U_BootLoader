#ifndef __PLATFORMMANAGER_H
#define __PLATFORMMANAGER_H

#include "F28003x_Examples.h"
#include "Hw_AdcDriver.h"


#ifdef __cplusplus
    extern "C" {
#endif

#define MODESCIID0_MODBUS   0
#define MODESCIID0_XMODEM   1




typedef struct
{
    Uint16 ModeSciID0           :2;     // 0:Modbus     1:Xmodem
    Uint16 ModbusTimeOutFlag    :1;     // 0:通信正常   1:通信异常
    Uint16 PlatformIsInit       :1;     // 0:未完成初始化     1:已完成初始化
    Uint16 bRsv                 :12;    // 保留

}SYS_PLATFORM_CTRL;


static inline Uint16 GetModeSciID0(SYS_PLATFORM_CTRL *pObjPlatformCtrl)
{
    return pObjPlatformCtrl->ModeSciID0;
}

static inline void SetModeSciID0(SYS_PLATFORM_CTRL *pObjPlatformCtrl, Uint16 Mode)
{
    pObjPlatformCtrl->ModeSciID0 = Mode;
}


static inline void SetModbusTimeOutFlag(SYS_PLATFORM_CTRL *pObjPlatformCtrl)
{
    pObjPlatformCtrl->ModbusTimeOutFlag = 1;
}

static inline void ClearModbusTimeOutFlag(SYS_PLATFORM_CTRL *pObjPlatformCtrl)
{
    pObjPlatformCtrl->ModbusTimeOutFlag = 0;
}


static inline Uint16 GetPlatformIsInit(SYS_PLATFORM_CTRL *pObjPlatformCtrl)
{
    return pObjPlatformCtrl->PlatformIsInit;
}


static inline void SetPlatformInit(SYS_PLATFORM_CTRL *pObjPlatformCtrl)
{
    pObjPlatformCtrl->PlatformIsInit = 1;
}


extern SYS_PLATFORM_CTRL ObjPlatformCtrl;

extern void TaskLed(void);







#ifdef __cplusplus
    }
#endif




#endif
