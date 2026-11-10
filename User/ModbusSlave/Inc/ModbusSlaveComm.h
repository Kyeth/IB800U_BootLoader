#ifndef __MODBUSSLAVECOMM_H
#define __MODBUSSLAVECOMM_H

#include "ModbusSlave.h"


#ifdef __cplusplus
extern "C" {
#endif



extern void ModbusSlaveInit(void);
extern void TaskModbusSlave_1(void);


extern ST_MODBUS_SLAVE_TABLE_CRTL ObjModbusSlaveCtrl_1;    // Modbus从站对象

#ifdef __cplusplus
}
#endif


#endif
