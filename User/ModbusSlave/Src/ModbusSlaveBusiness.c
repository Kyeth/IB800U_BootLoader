#include "ModbusSlaveBusiness.h"
#include "ModbusSlaveComm.h"


#ifdef __cplusplus
extern "C" {
#endif

void ModbusSlaveCallBack(void)
{
    Uint16 u16FirmwareType = ObjModbusSlaveCtrl_1.stMdsUpdateParam.u16FirmwareType;



    if (((ObjModbusSlaveCtrl_1.stMdsFrame.bAddr == ObjModbusSlaveCtrl_1.bModbusSlaveAddr)    // 是否符合本机地址
            || (ObjModbusSlaveCtrl_1.stMdsFrame.bAddr == 0x00) )                             // 是否符合广播地址
            && (ObjModbusSlaveCtrl_1.stMdsFrame.u16RegStartAddr == 1080)                    // 是否符合升级地址
            && (ObjModbusSlaveCtrl_1.stMdsFrame.u16RegNums == 2)                            // 是否寄存器长度等于2
            && (u16FirmwareType >=0 && u16FirmwareType <=3))                                // 是否符合接收的固件类型
    {
        ObjModbusSlaveCtrl_1.stMdsUpdateParam.u16MdsIsUpdate = 1;
    }
}

#ifdef __cplusplus
}
#endif
