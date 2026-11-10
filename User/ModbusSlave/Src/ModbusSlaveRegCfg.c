#include "ModbusSlaveRegCfg.h"
#include "Hw_AdcDriver.h"
#include "PlatformManager.h"
#include "ModbusSlaveComm.h"
#include "Hw_SysCpuTimer.h"
#include "VersionUser.h"


#ifdef __cplusplus
extern "C" {
#endif

#define MDS_ITEM_OF(arr)            (sizeof(arr) / sizeof(arr[0]))

Uint16 g_u16TestArry[82] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

extern Uint16 u16Prd;
extern Uint16 u16PrdBak;
extern Uint16 u16DbTime;
extern Uint16 u16DbTimeBak;
float fValue = 60;
int32 i32Value = 40;

/*******************************************************************************************************************************************************************************************************************************************************************************************************************
   (寄存器地址保持增序!!!)                                                                                信号全局动态配置表
********************************************************************************************************************************************************************************************************************************************************************************************************************
  寄存器地址         读写类型             数据类型         保存类型            值指针         值下限           值上限
********************************************************************************************************************************************************************************************************************************************************************************************************************/
const ST_REG_SLAVE_ITEM g_stRegSlaveItem[] =
{
     {0,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[0]),        0,    65535},
     {1,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[1]),        0,    65535},
     {2,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[2]),        0,    65535},
     {3,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[3]),        0,    65535},
     {4,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[4]),        0,    65535},
     {5,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[5]),        0,    65535},
     {6,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[6]),        0,    65535},
     {7,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[7]),        0,    65535},
     {8,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[8]),        0,    65535},
     {9,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[9]),        0,    65535},
     {10,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[10]),        0,    65535},
     {11,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[11]),        0,    65535},
     {12,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[12]),        0,    65535},
     {13,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[13]),        0,    65535},
     {14,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[14]),        0,    65535},
     {15,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[15]),        0,    65535},
     {16,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[16]),        0,    65535},
     {17,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[17]),        0,    65535},

     {19,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[19]),        0,    65535},
     {20,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[20]),        0,    65535},
     {21,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[21]),        0,    65535},
     {22,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[22]),        0,    65535},
     {23,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[23]),        0,    65535},
     {24,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[24]),        0,    65535},
     {25,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[25]),        0,    65535},
     {26,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[26]),        0,    65535},
     {27,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[27]),        0,    65535},
     {28,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[28]),        0,    65535},
     {29,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[29]),        0,    65535},
     {30,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[30]),        0,    65535},
     {31,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[31]),        0,    65535},
     {32,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[32]),        0,    65535},
     {33,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[33]),        0,    65535},
     {34,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[34]),        0,    65535},
     {35,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[35]),        0,    65535},
     {36,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[36]),        0,    65535},
     {37,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[37]),        0,    65535},
     {38,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[38]),        0,    65535},
     {39,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[39]),        0,    65535},
     {40,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[40]),        0,    65535},
     {41,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[41]),        0,    65535},
     {42,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[42]),        0,    65535},
     {43,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[43]),        0,    65535},
     {44,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[44]),        0,    65535},
     {45,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[45]),        0,    65535},
     {46,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[46]),        0,    65535},
     {47,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[47]),        0,    65535},
     {48,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[48]),        0,    65535},
     {49,    READ_WRITE,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[49]),        0,    65535},

     {61,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[61]),        0,    65535},
     {62,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[62]),        0,    65535},
     {63,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[63]),        0,    65535},
     {64,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[64]),        0,    65535},
     {65,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[65]),        0,    65535},
     {66,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[66]),        0,    65535},
     {67,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[67]),        0,    65535},
     {68,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[68]),        0,    65535},
     {69,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[69]),        0,    65535},
     {70,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[70]),        0,    65535},
     {71,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[71]),        0,    65535},
     {72,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[72]),        0,    65535},
     {73,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[73]),        0,    65535},
     {74,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[74]),        0,    65535},
     {75,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[75]),        0,    65535},
     {76,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[76]),        0,    65535},
     {77,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[77]),        0,    65535},
     {78,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[78]),        0,    65535},
     {79,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[79]),        0,    65535},
     {80,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[80]),        0,    65535},
     {81,    READ_ONLY,     TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&g_u16TestArry[81]),        0,    65535},

     {1080,    READ_WRITE,    TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&ObjModbusSlaveCtrl_1.stMdsUpdateParam.u16FirmwareType),        0,   1},
     {1081,    READ_WRITE,    TYPE_UINT16,    NOT_SAVE,  (U_REG_VAL *)(&ObjModbusSlaveCtrl_1.stMdsUpdateParam.u16UpdateBaud),        0,   3},




};
const ST_REG_SLAVE_CFG stRegSlaveCfg = {(ST_REG_SLAVE_ITEM*)&g_stRegSlaveItem, MDS_ITEM_OF(g_stRegSlaveItem)};




#ifdef __cplusplus
}
#endif
