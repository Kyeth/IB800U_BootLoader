#ifndef __PARAMSAVE_H
#define __PARAMSAVE_H

#include "RingBuff.h"


#ifdef __cplusplus
extern "C" {
#endif



#define PARAM_MANAGER_ADDR  128

#define PARAM_START_ADDR  (PARAM_MANAGER_ADDR+8)

#define CMD_SAVE_SING_PARAM     0x01
#define CMD_SAVE_ALL_PARAM      0x02
#define CMD_LOAD_SING_PARAM     0x04
#define CMD_LOAR_ALL_PARAM      0x08



// 寄存器对应的值的类型
typedef enum
{
    PARAM_TYPE_INT8,
    PARAM_TYPE_UINT8,
    PARAM_TYPE_INT16,
    PARAM_TYPE_UINT16,
    PARAM_TYPE_INT32,
    PARAM_TYPE_UINT32,
    PARAM_TYPE_FLOAT
}E_PARAM_TYPE;




//  联合体
typedef union
{
    unsigned char   u8Val;
    char            i8Val;
    Uint16  u16Val;
    int16   i16Val;
    Uint32  u32Val;
    int32   i32Val;
    float   fVal;
}U_PARAM_VAL;

typedef struct
{
    int (*ParamWrite)(Uint32 u32Addr, Uint16 *u16Buff, Uint16 u16Size);
    int (*ParamRead)(Uint32 u32Addr, Uint16 *u16Buff, Uint16 u16Size);
}ST_PARAM_WR_INTF;


typedef struct
{
    Uint16 u16Index;
    Uint16 bIsSaveFlag      :1;
    Uint16 bParamType       :4;
    Uint16 Rsv              :11;
    U_PARAM_VAL *puParamVal;
}ST_PARAM_ITEM;

typedef struct
{
    ST_PARAM_ITEM *pstParamItem;
    Uint16 u16ItemNum;
}ST_PARAM_CFG;


typedef struct
{
    ST_PARAM_CFG *pstParamCfg;
    Uint16 u16SingleParamLoadBuff[10];
    Uint16 u16SingleParamSaveBuff[100];
    ST_RING_BUFF stSingleParamLoad;
    ST_RING_BUFF stSingleParamSave;
    ST_PARAM_WR_INTF stParamWRIntf;
    Uint16 bCmdParam :8;
    Uint16 Rsv       :8;
}ST_PARAM_SAVE_HANDLE;


#ifdef __cplusplus
}
#endif

#endif
