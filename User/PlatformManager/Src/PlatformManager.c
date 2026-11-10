#include "PlatformManager.h"
#include "Hw_GpioDriver.h"
#include "Flash280039C.h"

SYS_PLATFORM_CTRL ObjPlatformCtrl;




void TaskLed(void)
{

//    if ((GetModeSciID0(&ObjPlatformCtrl) == MODESCIID0_XMODEM))
//    {
//        Hw_GpioTogglePin(34);
//        Hw_GpioTogglePin(6);
//    }
//    else
//    {
//        if ((GetModeSciID0(&ObjPlatformCtrl) == MODESCIID0_MODBUS) && (stDsp280039Flash.eFlashState == FLASH_IDLE))
//        {
//            Hw_GpioTogglePin(34);
//            Hw_GpioTogglePin(6);
//        }
//        else
//        {
//            Hw_GpioWritePin(34, 1);
//            Hw_GpioWritePin(6, 0);
//        }
//    }

}
