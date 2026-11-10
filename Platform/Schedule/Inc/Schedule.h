//#############################################################################
//
// FILE:   Schedule.h
//
// TITLE:  閺夊灝鐡欓惃鍕�栨禒鑸靛▕鐠炩槄绱欓崢鐔烘倞閸ュ墽娈戞潪顖欐閹诲繗鍫弬鍥︽閿涳拷
//
//#############################################################################


#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//
#include "driverlib.h"

//
// Check for invalid compile flags
//
#if defined(__TMS320C28XX_FPU64__)
#error "Invalid FPU Configuration"
#endif


// 任务控制对象
typedef struct tagTaskCtrl
{
    uint8_t On;         // 周期达到
    uint8_t Num;        // 已注册的任务数
}TTaskCtrl;


typedef struct tagTask
{
    uint16_t Time;           // 周期值
    uint16_t Cnt;            // 计数值
    void (*Callback)(void);  // 回调函数
}TTask;
//----------------------------------------------------
extern TTaskCtrl TaskCTL;
extern TTask Task[];

extern void Task_Start(void (*task)(void), void* pArgV, uint16_t time, uint16_t point);

extern void Task_Run();

#ifdef __cplusplus
}
#endif

#endif // __DEVICE_H__
