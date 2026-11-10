/**
 * @file main.c
 * @author hanyuhang (008467)
 * @brief 
 * @version 0.1
 * @date 2025-06-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//#include "cpu_cla.h"
#include <Device.h>
#include "ModbusSlaveComm.h"
#include "driverlib.h"
#include "device.h"
#include "f28x_project.h"
#include "Hw_SysCpuTimer.h"
#include "Schedule.h"
#include "InterruptCfg.h"
#include "DrvCfg_IB800.h"
#include "XmodemSlaveProcess.h"
#include "Hw_WatchDog.h"
#include "PlatformManager.h"
#include "Hw_MCanDriver.h"
#include "DspUpgradeCrc.h"
#include "Flash280039C.h"
#include "UpgradeUser.h"
#include "VersionUser.h"

/**
 * @brief 芯片外设初始化
 * 
 */
void PeripheralsInit(void)
{
    //---------------------------------------------------------------------------
    EALLOW;
    DINT;   // Disable Global interrupt INTM
    EDIS;
    //---------------------------------------------------------------------------

    // 初始化看门狗超时时间为5ms
    Hw_WatchDogInit(5);

    // 初始化系统时钟
    Device_init();

    // 解锁所有GPIO
    Device_initGPIO();

    // 配置中断复位函数
    InterruptVectTableInit();

    Hw_DisableWatchDog();


    // 这里配置一个CPUTime1
    Hw_CpuTimerInit((ST_SYS_TIMER *)&stSysCpuTimer1);

    Hw_CpuTimerInit((ST_SYS_TIMER *)&stSysCpuTimer2);

    // 初始化外设
    Hw_GpioInit(&stGpioCfg);

    Hw_SciInit(&stSciCfg);


    // 开启定时器
    // 这里开启定时器的目的是为了使得通信相关接口可以正常工作
    Hw_CpuTimerStart((ST_SYS_TIMER *)&stSysCpuTimer2);


    //---------------------------------------------------------------------------
    EALLOW;
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global real-time interrupt DBGM
    EDIS;
    //---------------------------------------------------------------------------
}



/**
 * @brief 应用层数据对象初始化
 * 
 */
void ApplicationInit(void)
{

}



/**
 * @brief 平台组件功能初始化
 * 
 */
void PlatformInit(void)
{
    // 软件版本初始化
    VersionUserInit();

//    // Modbus从站初始化
//    ModbusSlaveInit();

    // Xmodem从站初始化
    XmodemProcessInit();

    Dsp280039FlashInit();

    UpgradeUserInit();

}


/**
 * @brief 任务创建
 * 
 * 任务中不允许存在死循环，一定需要存在退出条件
 * 否则会触发看门狗复位
 */
void TaskCreate(void)
{
    // Upgrade升级任务
    Task_Start(&TaskUpgrade, NULL, 10, 0);

    // Upgrade升级任务
    Task_Start(&TaskLed, NULL, 1000, 0);
}



/**
 * @brief 启动系统
 * 
 */
void StartSystem(void)
{

    Hw_CpuTimerStart((ST_SYS_TIMER *)&stSysCpuTimer1);

    // 标记系统平台完成初始化，中断相关函数的逻辑可以执行
    SetPlatformInit(&ObjPlatformCtrl);

    // 上电就进入Xmodem接收模式
    SetModeSciID0(&ObjPlatformCtrl, MODESCIID0_XMODEM);

}



void main(void)
{

    // 初始化外设
    PeripheralsInit();

    // 应用层数据对象初始化
    ApplicationInit();

    // 平台组件功能初始化
    PlatformInit();

    // 创建所有任务
    TaskCreate();

    // 校验升级信息
    CheckUpgrade();

    // 启动系统
    StartSystem();
    while(1)
    {

        Task_Run();
    }
}

