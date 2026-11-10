#include "FM24C128D.h"
#include "Hw_GpioDriver.h"
#include "Hw_I2cDriver.h"

#ifdef __cplusplus
    extern "C" {
#endif


ST_FM24C128D_HANDLE stFM24C128DHandle;


void FM24C128DEnableWriteProtection(ST_FM24C128D_HANDLE *pstFM24C128DHandle)
{
    Hw_GpioWritePin(pstFM24C128DHandle->u16WRPort, 1);
}



void FM24C128DDisableWriteProtection(ST_FM24C128D_HANDLE *pstFM24C128DHandle)
{
    Hw_GpioWritePin(pstFM24C128DHandle->u16WRPort, 0);
}



// 初始化，需要检测ID能否读到
int16 FM24C128DRegister(ST_FM24C128D_HANDLE *pstFM24C128DHandle, ST_FM24C128D_INTF *pstFM24C128DRwIntf, Uint16 u16Port, Uint16 u16Addr)
{
    if ((pstFM24C128DHandle == NULL) || (pstFM24C128DRwIntf == NULL) || (pstFM24C128DRwIntf->EepromRead == NULL)
            || (pstFM24C128DRwIntf->EepromRWRequest == NULL))
    {
        return -1;
    }
    pstFM24C128DHandle->stFM24C128DIntf.EepromRead = pstFM24C128DRwIntf->EepromRead;
    pstFM24C128DHandle->stFM24C128DIntf.EepromRWRequest = pstFM24C128DRwIntf->EepromRWRequest;
    pstFM24C128DHandle->u16WRPort = u16Port;
    pstFM24C128DHandle->u16Addr = u16Addr;

    pstFM24C128DHandle->IsInit = 1;
    return 0;
}


// 写操作
// 字节写入
int16 FM24C128DWriteByte(ST_FM24C128D_HANDLE *pstFM24C128DHandle, Uint16 u16TargetAddr, Uint16 u16Data)
{
    int16 iRet = -1;
    ST_FM24_I2C_MSG stI2cMsg;
    if ((pstFM24C128DHandle == NULL) || (pstFM24C128DHandle->IsInit == 0))
    {
        return -1;
    }

    stI2cMsg.bAddr = pstFM24C128DHandle->u16Addr;
    stI2cMsg.bDir = 0;
    stI2cMsg.bMsgId = 1;    // 写消息不需要ID
    stI2cMsg.u16TargetBuff[0] = u16Data;
    stI2cMsg.u16DataLength = 1;
    stI2cMsg.u32WrAddr = u16TargetAddr;

    pstFM24C128DHandle->stFM24C128DIntf.EepromRWRequest(pstFM24C128DHandle, &stI2cMsg);

    return iRet;
}


// 检测设备是否就绪
int16 FM24C128DIsReady(ST_FM24C128D_HANDLE *pstFM24C128DHandle)
{
    Uint16 u16TargetAddr = 0;
    Uint16 u16ReadBuff[10] = 0;
    int16 iRet = -1;
    if ((pstFM24C128DHandle == NULL) || (pstFM24C128DHandle->IsInit == 0))
    {
        return -1;
    }

    // 这里需要传入目标地址，数据Buff，读写长度
    pstFM24C128DHandle->stFM24C128DIntf.EepromRead(pstFM24C128DHandle, u16ReadBuff);

    if (1)
    {
        iRet = 0;
    }
    else
    {
        iRet = 1;
    }

    return iRet;
}


// 页写入
// 还需要考虑跨页的情况
int16 FM24C128DWritePage(ST_FM24C128D_HANDLE *pstFM24C128DHandle, Uint16 u16TargetAddr, Uint16 *u16DataBuff, Uint16 u16Len)
{
    int16 iRet = -1;

    int16 u16Page1 = 0, u16Page2 = 0, u16PageOffset = 0;;

    if ((pstFM24C128DHandle == NULL) || (pstFM24C128DHandle->IsInit == 0) || (u16DataBuff == NULL))
    {
        return -1;
    }

//    if (u16Len > FM24C128D_PAGE_SIZE)
//    {
//        return -2;
//    }

    u16Page1  = u16TargetAddr / FM24C128D_PAGE_SIZE;
    u16Page2 = (u16TargetAddr + u16Len) / FM24C128D_PAGE_SIZE;


//    stI2cMsg.bAddr = pstFM24C128DHandle->u16Addr;
//    stI2cMsg.bDir = 0;
//    stI2cMsg.bMsgId = 1;    // 写消息不需要ID
//    memcpy(&stI2cMsg.u16TargetBuff[0], u16DataBuff, u16Len);
//    stI2cMsg.u16DataLength = u16Len;
//    stI2cMsg.u32WrAddr = u16TargetAddr;
//
//    iRet = pstFM24C128DHandle->stFM24C128DIntf.EepromRWRequest(pstFM24C128DHandle, &stI2cMsg);

    return iRet;
}

// 读操作
// 随机读
int16 FM24C128DReadByte(ST_FM24C128D_HANDLE *pstFM24C128DHandle, Uint16 u16TargetAddr, Uint16 *pu16Data)
{
    int16 iRet = -1;
    ST_FM24_I2C_MSG stI2cMsg;

    if ((pstFM24C128DHandle == NULL) || (pstFM24C128DHandle->IsInit == 0))
    {
        return -1;
    }

    stI2cMsg.bAddr = pstFM24C128DHandle->u16Addr;
    stI2cMsg.bDir = 1;
    stI2cMsg.bMsgId = 2;    // 这个ID得想办法处理
    stI2cMsg.u16DataLength = 1;
    stI2cMsg.u32WrAddr = u16TargetAddr;

    pstFM24C128DHandle->stFM24C128DIntf.EepromRWRequest(pstFM24C128DHandle, &stI2cMsg);

    while (1)
    {

        // 读取数据
        iRet = pstFM24C128DHandle->stFM24C128DIntf.EepromRead(pstFM24C128DHandle, &stI2cMsg);

        if (iRet)
        {
            pu16Data[0] = stI2cMsg.u16TargetBuff[0];
            break;
        }
    }

    return iRet;
}

// 顺序读
int16 FM24C128DReadPage(ST_FM24C128D_HANDLE *pstFM24C128DHandle, Uint16 u16TargetAddr, Uint16 *u16DataBuff, Uint16 u16Len)
{
    int16 iRet = -1;
    if ((pstFM24C128DHandle == NULL) || (pstFM24C128DHandle->IsInit == 0))
    {
        return -1;
    }

    if (u16Len > FM24C128D_PAGE_SIZE)
    {
        return -2;
    }

//    iRet = pstFM24C128DHandle->stFM24C128DIntf.EepromRead(pstFM24C128DHandle, u16DataBuff, u16Len);

    if (1)
    {
        iRet = 0;
    }
    else
    {
        iRet = 1;
    }
    return iRet;
}



int16 FM24C128DReadUID(ST_FM24C128D_HANDLE *pstFM24C128DHandle, Uint16 *pu16Data)
{
    int16 iRet = -1;
    Uint32 u32TimeOut = 0;
    ST_FM24_I2C_MSG stI2cMsg;

    if ((pstFM24C128DHandle == NULL) || (pstFM24C128DHandle->IsInit == 0))
    {
        return -1;
    }

    stI2cMsg.bAddr = FM24C128D_UID_ADDR;        // 读取UID地址和读取数据地址不同，在初始化时配置的是数据地址
    stI2cMsg.bDir = 1;                          // 读命令请求
    stI2cMsg.bMsgId = 0;
    stI2cMsg.u32WrAddr = 0x0200;
    stI2cMsg.u16DataLength = 16;

    // 下发读请求命令
    pstFM24C128DHandle->stFM24C128DIntf.EepromRWRequest(pstFM24C128DHandle, &stI2cMsg);

    while (1)
    {

        // 读取数据
        iRet = pstFM24C128DHandle->stFM24C128DIntf.EepromRead(pstFM24C128DHandle, &stI2cMsg);

        if (iRet)
        {
            memcpy(pu16Data, &stI2cMsg.u16TargetBuff[0], 16);
            break;
        }
    }
    return iRet;
}


// 应答查询


#ifdef __cplusplus
    }
#endif

