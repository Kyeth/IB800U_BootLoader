//#############################################################################
//
// FILE:   Schedule.c
//
// TITLE:  閺夊灝鐡欓惃鍕�栨禒鑸靛▕鐠炩�崇湴鐎圭偟骞囬弬鍥︽.
//
//#############################################################################


//#include "device.h"
//#include "driverlib.h"
#include "Schedule.h"

//---------------------------------------------------------------------
// 最大任务数
#define TASK_MAX_NUM 17


//----------------------------------------------------------------------

TTaskCtrl TaskCTL = {0,0};
TTask Task[TASK_MAX_NUM];
//---------------------------------------------------------------------


void Task_Start(void (*task)(void), void* pArgV, uint16_t time, uint16_t point)
{
    if(TaskCTL.Num < TASK_MAX_NUM)
    {
        Task[TaskCTL.Num].Callback = task;
        Task[TaskCTL.Num].Time = time;
        Task[TaskCTL.Num].Cnt = point;   //  起始偏移
        TaskCTL.Num++;
    }
}

void Task_Run()
{
    if(TaskCTL.On)
    {
        uint8_t Scan = 0;
        for(Scan = 0; Scan < TaskCTL.Num; Scan++)
        {
            Task[Scan].Cnt++;
            if(Task[Scan].Cnt >= Task[Scan].Time)
            {
                Task[Scan].Cnt = 0;
                (*(Task[Scan].Callback))();
            }
        }
        TaskCTL.On = 0;
    }
}
