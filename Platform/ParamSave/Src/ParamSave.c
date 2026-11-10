#include "ParamSave.h"

#ifdef __cplusplus
extern "C" {
#endif

static int16 BinarySearch(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle, Uint16 u16Index)
{
    Uint16 u16Len = 0, u16Low = 0, u16High = 0, u16Mid = 0;
    int32 i32RingLen = 0, i;

    ST_PARAM_ITEM *pstParamStart = NULL;
    ST_PARAM_ITEM *pstParamTmp = NULL;
    ST_PARAM_ITEM *pstParamMix = NULL;
    ST_PARAM_ITEM *pstParamMax = NULL;

    if (pstParamSaveHandle == NULL)
    {
        return -1;
    }

    pstParamMix = (ST_PARAM_ITEM*)&pstParamSaveHandle->pstParamCfg->pstParamItem[0];
    pstParamMax = (ST_PARAM_ITEM*)&pstParamSaveHandle->pstParamCfg->pstParamItem[pstParamSaveHandle->pstParamCfg->u16ItemNum-1];

    if ((u16Index < pstParamMix->u16Index) || (u16Index > pstParamMax->u16Index))
    {
        return -2;
    }

    pstParamStart = pstParamSaveHandle->pstParamCfg->pstParamItem;
    u16Len = pstParamSaveHandle->pstParamCfg->u16ItemNum;

    u16High = u16Len - 1;

    while (u16Low <= u16High)
    {
        u16Mid = (u16High + u16Low) >> 1;
        pstParamTmp = pstParamStart + u16Mid;

        if (u16Index < pstParamTmp->u16Index)
        {
            u16High = u16Mid - 1;
        }
        else if (u16Index > pstParamTmp->u16Index)
        {
            u16Low = u16Mid + 1;
        }
        else
        {
            return u16Mid;
        }
    }
    return 0;
}

static int16 SetParamValue(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle, void* vBuff, Uint16 u16Index)
{
    if ((pstParamSaveHandle == NULL) || (vBuff == NULL))
    {
        return -1;
    }

    switch (pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].bParamType)
    {
    case PARAM_TYPE_INT8:
        pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->i8Val = *(int16 *)vBuff;
        break;

    case PARAM_TYPE_UINT8:
        pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->u8Val = *(Uint16 *)vBuff;
        break;

    case PARAM_TYPE_INT16:
        pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->i16Val = *(int16 *)vBuff;
        break;

    case PARAM_TYPE_UINT16:
        pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->u16Val = *(Uint16 *)vBuff;
        break;

    case PARAM_TYPE_INT32:
        pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->i32Val = *(int32 *)vBuff;
        break;

    case PARAM_TYPE_UINT32:
        pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->u32Val = *(Uint32 *)vBuff;
        break;

    case PARAM_TYPE_FLOAT:
        pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->fVal = *(float *)vBuff;
        break;

    default:
        pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->u8Val = *(Uint16 *)vBuff;
        break;
    }
}



static int16 GetParamValue(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle, void* vBuff, Uint16 u16Index)
{
    if ((pstParamSaveHandle == NULL) || (vBuff == NULL))
    {
        return -1;
    }

    switch (pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].bParamType)
    {
    case PARAM_TYPE_INT8:
        *(int16 *)vBuff = pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->i8Val;
        break;

    case PARAM_TYPE_UINT8:
        *(Uint16 *)vBuff = pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->u8Val;
        break;

    case PARAM_TYPE_INT16:
        *(int16 *)vBuff = pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->i16Val;
        break;

    case PARAM_TYPE_UINT16:
        *(Uint16 *)vBuff = pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->u16Val;
        break;

    case PARAM_TYPE_INT32:
        *(int32 *)vBuff = pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->i32Val;
        break;

    case PARAM_TYPE_UINT32:
        *(Uint32 *)vBuff = pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->u32Val;
        break;

    case PARAM_TYPE_FLOAT:
        *(float *)vBuff = pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->fVal;
        break;

    default:
        *(Uint16 *)vBuff = pstParamSaveHandle->pstParamCfg->pstParamItem[u16Index].puParamVal->u16Val;
        break;
    }
}



int16 ParamSaveInit(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle, ST_PARAM_CFG *pstParamCfg, ST_PARAM_WR_INTF *pstParamWrIntf)
{
    if ((pstParamSaveHandle == NULL) || (pstParamCfg == NULL))
    {
        return -1;
    }


    pstParamSaveHandle->pstParamCfg = pstParamCfg;

    InitRingBuff(&pstParamSaveHandle->stSingleParamLoad, pstParamSaveHandle->u16SingleParamLoadBuff, sizeof(Uint16), 10);
    InitRingBuff(&pstParamSaveHandle->stSingleParamSave, pstParamSaveHandle->u16SingleParamSaveBuff, sizeof(Uint16), 100);

    pstParamSaveHandle->stParamWRIntf.ParamWrite = pstParamWrIntf->ParamWrite;
    pstParamSaveHandle->stParamWRIntf.ParamRead = pstParamWrIntf->ParamRead;
    return 1;
}



int16 LoadAllParam(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle)
{
    Uint16 u16Buff[2];

    union unValue
    {
        float fVal;
        Uint16 u16Val[2];
    } uFloatByte;

    if (pstParamSaveHandle == NULL)
    {
        return -1;
    }

    pstParamSaveHandle->bCmdParam = CMD_LOAR_ALL_PARAM | pstParamSaveHandle->bCmdParam;

    return 1;
}


int16 SaveAllParam(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle)
{
    Uint16 u16Buff[2];

    union unValue
    {
        float fVal;
        Uint16 u16Val[2];
    } uFloatByte;

    if (pstParamSaveHandle == NULL)
    {
        return -1;
    }

    pstParamSaveHandle->bCmdParam = CMD_SAVE_ALL_PARAM | pstParamSaveHandle->bCmdParam;

    return 1;
}


int16 SaveSingleParam(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle, Uint16 u16Index)
{
    if (pstParamSaveHandle == NULL)
    {
        return -1;
    }

    // 参数写入缓存中
    if (WriteRingBuff(&pstParamSaveHandle->stSingleParamSave, &u16Index, 1))
    {
        pstParamSaveHandle->bCmdParam = CMD_SAVE_SING_PARAM | pstParamSaveHandle->bCmdParam;
        return 1;
    }
    else
    {
        return 0;
    }
}


int16 LoadSingleParam(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle, Uint16 u16Index)
{
    if (pstParamSaveHandle == NULL)
    {
        return -1;
    }

    // 参数写入缓存中
    if (WriteRingBuff(&pstParamSaveHandle->stSingleParamLoad, &u16Index, 1))
    {
        pstParamSaveHandle->bCmdParam = CMD_SAVE_SING_PARAM | pstParamSaveHandle->bCmdParam;
        return 1;
    }
    else
    {
        return 0;
    }

}


int16 LoadAllParamProc(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle)
{
    Uint16 i = 0;
    Uint32 u32Buff;

    union unValue
    {
        float fVal;
        Uint16 u16Val[2];
    } uFloatByte;

    if (pstParamSaveHandle == NULL)
    {
        return -1;
    }

    pstParamSaveHandle->bCmdParam = CMD_SAVE_SING_PARAM | pstParamSaveHandle->bCmdParam;

    for (i = 0; i < pstParamSaveHandle->pstParamCfg->u16ItemNum; i++)
    {
        pstParamSaveHandle->stParamWRIntf.ParamRead(PARAM_START_ADDR + pstParamSaveHandle->pstParamCfg->pstParamItem[i].u16Index,
                                                    (Uint16 *)u32Buff, 4);

        SetParamValue(pstParamSaveHandle, &u32Buff, i);

    }
}

int16 SaveAllParamProc(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle)
{
    Uint16 i = 0;
    Uint32 u32Buff;

    union unValue
    {
        float fVal;
        Uint16 u16Val[2];
    } uFloatByte;
    uFloatByte.fVal = 0;

    if (pstParamSaveHandle == NULL)
    {
        return -1;
    }

    for (i = 0; i < pstParamSaveHandle->pstParamCfg->u16ItemNum; i++)
    {

        GetParamValue(pstParamSaveHandle, &u32Buff, i);

        pstParamSaveHandle->stParamWRIntf.ParamWrite(PARAM_START_ADDR + pstParamSaveHandle->pstParamCfg->pstParamItem[i].u16Index,
                                                     (Uint16 *)u32Buff, 4);
    }
}

int16 LoadSingleParamProc(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle)
{
    int32 i32RingLen, i, i32Result;
    Uint16 u16Index = 0;
    Uint32 u32Value = 0;

    Uint16 u16Buff[2] = {0};

    if (pstParamSaveHandle == NULL)
    {
        return -1;
    }

    i32RingLen = GetRingBuffDataLen(&pstParamSaveHandle->stSingleParamLoad);

    for (i = 0; i < i32RingLen; i++)
    {
        ReadRingBuff(&pstParamSaveHandle->stSingleParamLoad, &u32Value, 1);

        i32Result = BinarySearch(pstParamSaveHandle, u32Value);

        if (i32Result)
        {
            pstParamSaveHandle->stParamWRIntf.ParamRead(PARAM_START_ADDR + pstParamSaveHandle->pstParamCfg->pstParamItem[i32Result].u16Index,
                                                        (Uint16 *)u32Value, 4);

            SetParamValue(pstParamSaveHandle, u32Value, i32Result);
        }


    }
    return -3;

}




int16 SaveSingleParamProc(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle)
{

    Uint32 u32Buff;
    int32 i32RingLen = 0, i, i32Result;


    if (pstParamSaveHandle == NULL)
    {
        return -1;
    }

    i32RingLen = GetRingBuffDataLen(&pstParamSaveHandle->stSingleParamSave);

    for (i = 0; i < i32RingLen; i++)
    {
        ReadRingBuff(&pstParamSaveHandle->stSingleParamSave, &u32Buff, 1);

        i32Result = BinarySearch(pstParamSaveHandle, u32Buff);

        if (i32Result)
        {
            GetParamValue(pstParamSaveHandle, &u32Buff, i32Result);

            pstParamSaveHandle->stParamWRIntf.ParamWrite(PARAM_START_ADDR + pstParamSaveHandle->pstParamCfg->pstParamItem[i32Result].u16Index,
                                                         (Uint16 *)u32Buff, 4);
        }

    }


    return -3;

}

// 这个在应用层实现
int16 Handle(ST_PARAM_SAVE_HANDLE *pstParamSaveHandle)
{
    int16 i16Ret = 0;
    if (pstParamSaveHandle == NULL)
    {
        return -1;
    }

    if (pstParamSaveHandle->bCmdParam & CMD_SAVE_ALL_PARAM)
    {
        i16Ret = SaveAllParamProc(pstParamSaveHandle);
    }
    else if (pstParamSaveHandle->bCmdParam & CMD_SAVE_SING_PARAM)
    {
        i16Ret = SaveSingleParamProc(pstParamSaveHandle);
    }

    if (pstParamSaveHandle->bCmdParam & CMD_LOAR_ALL_PARAM)
    {
        i16Ret = LoadAllParamProc(pstParamSaveHandle);
    }
    else if (pstParamSaveHandle->bCmdParam & CMD_LOAD_SING_PARAM)
    {
        i16Ret = LoadSingleParamProc(pstParamSaveHandle);
    }
    return i16Ret;
}


#ifdef __cplusplus
}
#endif
