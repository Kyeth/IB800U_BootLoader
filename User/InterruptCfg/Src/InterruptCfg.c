#include <Hw_CpuTimer.h>
#include "InterruptCfg.h"
#include "interrupt.h"
#include "hw_ints.h"
#include "Hw_CanDriver.h"
#include "Hw_SciDriver.h"
#include "Hw_I2cDriver.h"
#include "Hw_AdcDriver.h"
#include "Hw_GpioDriver.h"
#include "Hw_SysCpuTimer.h"
#include "Hw_ClaDriver.h"
#include "Hw_MCanDriver.h"
#include "Schedule.h"
#include "f28003x_piectrl.h"
#include "f28003x_pie_defines.h"
#include "Hw_WatchDog.h"
#include "PlatformManager.h"
#include "Filter.h"
#include "Hw_PwmDriver.h"


extern void User_Can(void);

#ifdef __cplusplus
    extern "C" {
#endif


//-- CPUTimer1中断 --
#pragma CODE_SECTION(CpuTimer1Isr, ".ISR.ramfunc");
__interrupt void CpuTimer1Isr(void)
{
//    Hw_GpioWritePin(34, 0);
    // 这里清除CPU0Timer中断标志位
    CpuTimer1Regs.TCR.bit.TIF=1;
    ++stSysCpuTimer1.TimerDivCntPtd;
    if (stSysCpuTimer1.TimerDivCntPtd >= stSysCpuTimer1.TimerFreqDivCoef)
    {
        stSysCpuTimer1.TimerTick++;
        stSysCpuTimer1.TimerDivCntPtd = 0;
    }

//    // 系统平台完成初始化后才运行执行其他逻辑
//    if (GetPlatformIsInit(&ObjPlatformCtrl) == 1)
//    {
//
//    }
}


////
//#pragma CODE_SECTION(PWMIsr, ".ISR.ramfunc");
//__interrupt void PWMIsr(void)
//{
////    Hw_GpioWritePin(6, 0);
//
//    EPwm1Regs.ETCLR.bit.INT = 1;
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
//
//
//}



//-- CPUTimer2中断 --
#pragma CODE_SECTION(CpuTimer2Isr, ".TI.ramfunc");
__interrupt void CpuTimer2Isr(void)
{
//    Hw_GpioWritePin(34, 0);
    // 这里清除CPU0Timer中断标志位
    CpuTimer2Regs.TCR.bit.TIF=1;

    ++stSysCpuTimer2.TimerDivCntPtd;
    if (stSysCpuTimer2.TimerDivCntPtd >= stSysCpuTimer2.TimerFreqDivCoef)
    {
        stSysCpuTimer2.TimerTick++;
        stSysCpuTimer2.TimerDivCntPtd = 0;
    }
    // 确认时基为100us
    TaskCTL.On = 1;
    // 为了减少中断执行时间，把SCI和CAN通信进行了相位偏移调度
    // 在不同的中断周期中执行不同的函数
    switch (stSysCpuTimer2.TimerDivCntPtd)
    {
    case 0:
        // SCI通信
        Hw_IsrSciReceive(SCI_ID0);
        Hw_IsrSciTransmit(SCI_ID0);
    break;

    case 1:
        // CAN通信
//        Hw_IsrCanReceive(CAN_ID0);
//        Hw_IsrCanTransmit(CAN_ID0);
    break;

    case 2:
        // SCI通信
        Hw_IsrSciReceive(SCI_ID1);
        Hw_IsrSciTransmit(SCI_ID1);
    break;

    case 3:
//        Hw_IsrMCanTransmit(MCAN_ID0);
//        Hw_IsrMCanReceive(MCAN_ID0);
        break;

    default:
        break;
    }

//    // 系统平台完成初始化后才运行执行其他逻辑
//    if (GetPlatformIsInit(&ObjPlatformCtrl) == 1)
//    {
//
//    }

}


////-- AdcAIsr中断 --
//#pragma CODE_SECTION(AdcAIsr, ".TI.ramfunc");
//// 该中断在内存中运行，所有采样均进行滤波，总耗时9.85us
//__interrupt void AdcAIsr(void)
//{
//    // 清除ADC中断标志
//    Hw_AdcClearInitFlag(ADC_ID0);
//
//    ObjPlatformCtrl.u16AdcCnt++;
//    // 清除PIE中断标志
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
//}


//// 该中断为CLA任务处理结束后调用(不启用)
//__interrupt void TaskCla_1_Isr(void)
//{
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP11);
//}


//#pragma CODE_SECTION(Gpio47_Isr, ".TI.ramfunc");
//__interrupt void Gpio47_Isr(void)
//{
//
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
//}
//
//#pragma CODE_SECTION(Gpio49_Isr, ".TI.ramfunc");
//__interrupt void Gpio49_Isr(void)
//{
//
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
//}
//
//
//#pragma CODE_SECTION(Gpio53_Isr, ".TI.ramfunc");
//__interrupt void Gpio53_Isr(void)
//{
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP12);
//}


//-- 中断向量表初始化 --
void InterruptVectTableInit(void)
{
    // 清除中断标志，使能外设中断״̬
    Interrupt_initModule();

    // 复位中断向量表
    Interrupt_initVectorTable();

    // 注册Timer1的中断复位函数
    Interrupt_register(INT_TIMER1, CpuTimer1Isr);
    Interrupt_enable(INT_TIMER1);

    // 注册Timer2的中断复位函数
    Interrupt_register(INT_TIMER2, CpuTimer2Isr);
    Interrupt_enable(INT_TIMER2);

    // 注册ADCA的中断复位函数
//    Interrupt_register(INT_ADCA1, AdcAIsr);
//    Interrupt_enable(INT_ADCA1);

//    Interrupt_register(INT_EPWM1, PWMIsr);
//    Interrupt_enable(INT_EPWM1);

//    // 注册CLATask1任务完成中断
//    Interrupt_register(INT_CLA1_1, TaskCla_1_Isr);
//    Interrupt_enable(INT_CLA1_1);

//    // 注册GPIO47外部中断
//    Interrupt_register(INT_XINT1, Gpio47_Isr);
//    Interrupt_enable(INT_XINT1);
//
//    // 注册GPIO49外部中断
//    Interrupt_register(INT_XINT2, Gpio49_Isr);
//    Interrupt_enable(INT_XINT2);
//
//    // 注册GPIO53外部中断
//    Interrupt_register(INT_XINT3, Gpio53_Isr);
//    Interrupt_enable(INT_XINT3);
}

#ifdef __cplusplus
    }
#endif
