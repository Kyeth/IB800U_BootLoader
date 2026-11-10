#include "FM24C128DTramsport.h"
#include "Hw_I2cDriver.h"

#ifdef __cplusplus
    extern "C" {
#endif

int16 FM24C128DRead(ST_FM24C128D_HANDLE *pstHandle, ST_FM24_I2C_MSG *pstI2cMsg)
{
    int16 iRet = 0;
    Uint16 i = 0;
    ST_I2C_MSG stI2cMsg;

    stI2cMsg.bMsgId = pstI2cMsg->bMsgId;
    iRet = Hw_I2cRead(I2C_ID0, &stI2cMsg, 1);
    if (iRet)
    {
        memcpy(&pstI2cMsg->u16TargetBuff[0], &stI2cMsg.u16TargetBuff[0], stI2cMsg.u16DataLength);
        pstI2cMsg->bAddr = stI2cMsg.bAddr;
        pstI2cMsg->u16DataLength = stI2cMsg.u16DataLength;
        pstI2cMsg->u32WrAddr = stI2cMsg.u32WrAddr;
        pstI2cMsg->bMsgId = stI2cMsg.bMsgId;
        pstI2cMsg->bDir = stI2cMsg.bDir;
    }

    return iRet;
}

int16 FM24C128DRWRequest(ST_FM24C128D_HANDLE *pstHandle, ST_FM24_I2C_MSG *pstI2cMsg)
{
    int16 iRet = 0;
    Uint16 i = 0;
    ST_I2C_MSG stI2cMsg;

    stI2cMsg.bAddr = pstI2cMsg->bAddr;
    stI2cMsg.bDir = pstI2cMsg->bDir;
    stI2cMsg.bMsgId = pstI2cMsg->bMsgId;
    memcpy(&stI2cMsg.u16TargetBuff[0], &pstI2cMsg->u16TargetBuff[0], pstI2cMsg->u16DataLength);
    stI2cMsg.u16DataLength = pstI2cMsg->u16DataLength;
    stI2cMsg.u32WrAddr = pstI2cMsg->u32WrAddr;
    iRet = Hw_I2cSubmitRWReuest(I2C_ID0, &stI2cMsg);

    return iRet;
}


int16 FM24C128DGetTick(ST_FM24C128D_HANDLE *pstHandle)
{

}

#ifdef __cplusplus
    }
#endif

