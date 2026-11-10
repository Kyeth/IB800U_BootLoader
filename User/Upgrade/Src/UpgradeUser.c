#include "UpgradeUser.h"
#include "PlatformManager.h"
#include "Flash280039C.h"
#include "Hw_WatchDog.h"
#include "string.h"
#include "ModbusSlaveComm.h"
#include "Hw_SciDriver.h"
#include "XmodemCore.h"
#include "XmodemSlaveProcess.h"
#include "Dsp280039Flash.h"


ST_UPGRADE_HANDLE stUpgradeHandle;
//__attribute__((aligned(8))) Uint16 u16DataBuff[16];

void UpgradeUserInit(void)
{
    memset(&stUpgradeHandle, 0, sizeof(ST_UPGRADE_HANDLE));
    stUpgradeHandle.u32TimeOut = 50;
    stUpgradeHandle.u32TimeOutCnt = 0;
    stUpgradeHandle.u32UpgradeCfgAddr = 0xAF000U;
    stUpgradeHandle.u32AppAreaAddr = 0x86000U;
    stUpgradeHandle.u32BackUpAreaAddr = 0x9B000U;
    stUpgradeHandle.u32BootAreaAddr = 0x80000U;
}

void EraseBackUpBank(void)
{
    Uint16 i = 0;
    for (i = 0; i < 20; i++)
    {
        EraseSingleSector(stUpgradeHandle.u32BackUpAreaAddr + (i*0x1000));
    }
}

void EraseAppBank(void)
{
    Uint16 i = 0;
    for (i = 0; i < 21; i++)
    {
        EraseSingleSector(stUpgradeHandle.u32AppAreaAddr + (i*0x1000));
    }
}


void EraseBootBank(void)
{
    Uint16 i = 0;
    for (i = 0; i < 6; i++)
    {
        EraseSingleSector(stUpgradeHandle.u32BootAreaAddr + (i*0x1000));
    }
}

void jump_to_app(void)
{
    // APP区的起始地址
    FuncAppStart pFuncAppStart  = (FuncAppStart)0x00086000;

    EALLOW;
    DINT;   // Disable Global interrupt INTM
    EDIS;
    pFuncAppStart();
}

void CheckUpgrade(void)
{
    Uint32 u32Baud = 0;
    int16 i16Temp = 0;

    i16Temp = ReadUpgradeCfg(&stUpgradeHandle);
    if (i16Temp)
    {
        // 如果没有接收到完整的升级固件，则需要重新接收
        if (GetUpgradeFirmwareFlag(&stUpgradeHandle) == 0)
        {
            if (((stUpgradeHandle.stUpgradeCfg.u16UpgradeBaud == 96)
                    || (stUpgradeHandle.stUpgradeCfg.u16UpgradeBaud == 192)
                    || (stUpgradeHandle.stUpgradeCfg.u16UpgradeBaud == 384)
                    || (stUpgradeHandle.stUpgradeCfg.u16UpgradeBaud == 576)
                    || (stUpgradeHandle.stUpgradeCfg.u16UpgradeBaud == 1152)) &&
                    ((stUpgradeHandle.stUpgradeCfg.u16UpgradeUart == SCI_ID0 )
                            || (stUpgradeHandle.stUpgradeCfg.u16UpgradeUart == SCI_ID1)))
            {
                u32Baud = stUpgradeHandle.stUpgradeCfg.u16UpgradeBaud * 100;
                // 这里需要重新初始化串口波特率
                Hw_SciCtrl(stUpgradeHandle.stUpgradeCfg.u16UpgradeUart, SCI_COMM_CFG, &u32Baud);

                EraseBackUpBank();
            }
            else if (stUpgradeHandle.stUpgradeCfg.u16UpgradeBaud == 0)
            {
                u32Baud = 38400;
                // 这里需要重新初始化串口波特率
                Hw_SciCtrl(stUpgradeHandle.stUpgradeCfg.u16UpgradeUart, SCI_COMM_CFG, &u32Baud);

                EraseBackUpBank();
            }
            else
            {
                EraseSingleSector(stUpgradeHandle.u32UpgradeCfgAddr);
                jump_to_app();
            }
        }
        else    // 这里接收到完整的固件，需要进行固件校验与拷贝
        {
            if (VerifyFireware(&stUpgradeHandle))
            {

                if (stUpgradeHandle.stUpgradeCfg.bFirmwareType == 1)
                {
                    // 升级APP程序
                    EraseAppBank();
                    CopyAppFirmware(&stUpgradeHandle);
                    VerifyAppArea(&stUpgradeHandle);

                    EraseSingleSector(stUpgradeHandle.u32UpgradeCfgAddr);
                    jump_to_app();

                }
                else if (stUpgradeHandle.stUpgradeCfg.bFirmwareType == 2)
                {
                    // 升级Boot程序
                    EraseBootBank();
                    CopyBootFirmware(&stUpgradeHandle);
                    EraseSingleSector(stUpgradeHandle.u32UpgradeCfgAddr);
                    jump_to_app();
                }

            }
            else
            {
                EraseSingleSector(stUpgradeHandle.u32UpgradeCfgAddr);
                jump_to_app();
            }
        }
    }
    else
    {
        jump_to_app();
    }

}




void TaskUpgrade(void)
{
    Uint16 u16Timeout = 0;
    Uint32 u32Sum = 0;
    E_XMODEM_ERROR eXmodemError = XMODEM_OK;
    if ((ReadUpgradeCfg(&stUpgradeHandle) == 1) && (GetUpgradeFirmwareFlag(&stUpgradeHandle) == 0))
    {
        eXmodemError = XmodemReceive(&ObjXmodemCtrl);

        // 只有当 Xmodem 状态机完整结束（收到EOT并回复ACK）后才处理
        if (eXmodemError == XMODEM_END)
        {
            // 确认文件接收完整
            if (GetFileWriteOver(&ObjXmodemCtrl))
            {
                u16Timeout = 0;
                while ((GetRingBuffDataLen((ST_RING_BUFF *)&stSciHandle[SCI_ID1].stTxRingBuff) > 0) &&
                       (u16Timeout < 5000))
                {
                    Hw_IsrSciTransmit(SCI_ID1);
                    DELAY_US(100);
                    u16Timeout++;
                }

                u16Timeout = 0;
                while ((stSciHandle[SCI_ID1].pstSciRegs->SCIFFTX.bit.TXFFST > 0) && (u16Timeout < 5000))
                {
                    DELAY_US(100);
                    u16Timeout++;
                }

                u16Timeout = 0;
                while ((stSciHandle[SCI_ID1].pstSciRegs->SCICTL2.bit.TXEMPTY == 0) && (u16Timeout < 5000))
                {
                    DELAY_US(100);
                    u16Timeout++;
                }
                stUpgradeHandle.stUpgradeCfg.bChipType = ObjXmodemCtrl.stUpgradeFileHeader.bChipType;
                stUpgradeHandle.stUpgradeCfg.bFirmwareType = ObjXmodemCtrl.stUpgradeFileHeader.bFirmwareType;
                stUpgradeHandle.stUpgradeCfg.u16FirmwareOver = 0xAAEE;
                stUpgradeHandle.stUpgradeCfg.u32FileSize = ObjXmodemCtrl.stUpgradeFileHeader.u32FileSize;
                stUpgradeHandle.stUpgradeCfg.u32FileCrc = ObjXmodemCtrl.stUpgradeFileHeader.u32FileCrc;
                u32Sum = CalcUpgradeCfgSum(&stUpgradeHandle);
                stUpgradeHandle.stUpgradeCfg.u32UpgradeCfgSum = u32Sum;
                EraseSingleSector(stUpgradeHandle.u32UpgradeCfgAddr);
                WriteFlashData(stUpgradeHandle.u32UpgradeCfgAddr, &stUpgradeHandle.stUpgradeCfg, sizeof(ST_UPGRADE_CFG));

                // 写入配置信息后，重启
                Hw_EnableWatchDog();
                Hw_FeedWatchDog();
                Hw_SysReset();
            }
        }
        // 如果到达了重试上限，则退出升级
        if (eXmodemError == XMODEM_RETRY_ERROR)
        {
            EraseSingleSector(stUpgradeHandle.u32UpgradeCfgAddr);
            // 写入配置信息后，重启
            Hw_EnableWatchDog();
            Hw_FeedWatchDog();
            Hw_SysReset();
        }
    }
}
