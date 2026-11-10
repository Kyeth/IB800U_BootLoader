#include "ModbusSlaveComm.h"
#include "ModbusSlaveRegCfg.h"
#include "Modbus.h"
#include "ModbusSlave.h"
#include "Hw_SciDriver.h"
#include "PlatformManager.h"
#include "string.h"
#include "ModbusSlaveBusiness.h"

#define MODBUS_TIMEOUT  18000


ST_MODBUS_SLAVE_TABLE_CRTL ObjModbusSlaveCtrl_1;    // Modbus从站对象
static Uint16 MdsBuff[256] = {0};          // Modbus数据空间
static Uint16 ModbusTimeOutCnt = 0;        // Modbus超时统计计数器

void ModbusCallBackFunction(void)
{
    // 清空Modbus通信超时计数
    ModbusTimeOutCnt = 0;
}


void ModbusSlaveInit(void)
{
    // 将Modbus寄存器通信点表、回调函数注册进控制对象
    ModbusSlaveTableRegister(&ObjModbusSlaveCtrl_1,  MODBUS_SLAVE_DEFAULT,
                             (ST_REG_SLAVE_CFG *)&stRegSlaveCfg,
                             ModbusSlaveCallBack);

    // 设置当前通信使用的通信点表
    SetModbusSlaveTableType(&ObjModbusSlaveCtrl_1, MODBUS_SLAVE_DEFAULT);
    // 设置从机通信地址
    SetModbusSlaveAddr(&ObjModbusSlaveCtrl_1, 1);
}






void TaskModbusSlave_1(void)
{
    Uint16 ReceiveSize = 0;

    // 定义Modbus数据对象
    ST_MDS_DATA stMdsData;
    stMdsData.pu16DataBuff = MdsBuff;     // 给数据对象指针赋值实际数据空间
    stMdsData.u16DataLenth = 0;           // 清空接收数据的长度，在接收时：表示接收的数据长度，
                                        // 在发送时，表示发送的数据长度

    ST_MDS_FRAME *stMdsFrame = NULL;

    // 需要在初始化串口外设，通过在系统系统时已经初始化

    // SCI_ID0的通信模式为Modbus时才执行
    if (GetModeSciID0(&ObjPlatformCtrl) == MODESCIID0_MODBUS)
    {
        // 通信超时统计
        ModbusTimeOutCnt++;
        if (ModbusTimeOutCnt >= MODBUS_TIMEOUT)
        {
            ModbusTimeOutCnt = MODBUS_TIMEOUT;
            // 这里做一个通信超时标志
            SetModbusTimeOutFlag(&ObjPlatformCtrl);
        }
        else
        {
            ClearModbusTimeOutFlag(&ObjPlatformCtrl);
        }

        // 上一次调度是升级命令，在这一次调度才执行修改，避免修改波特率后应答帧发送失败
        if (ObjModbusSlaveCtrl_1.stMdsUpdateParam.u16MdsIsUpdate == 1)
        {
            SetModeSciID0(&ObjPlatformCtrl, MODESCIID0_XMODEM);

            // 这里修改波特率

            ObjModbusSlaveCtrl_1.stMdsUpdateParam.u16MdsIsUpdate = 0;
        }

        memset(stMdsData.pu16DataBuff, 0, 256);
        // 从串口接收Modbus帧
        ReceiveSize = Hw_SciRead(SCI_ID0, stMdsData.pu16DataBuff, 256);
        if (ReceiveSize)
        {
            stMdsData.u16DataLenth = ReceiveSize;

            // 调用Modbus从站协议
            stMdsFrame = ModbusSlaveHandle(&ObjModbusSlaveCtrl_1, &stMdsData);

            // 响应Modbus请求帧
            if(stMdsFrame)
            {
                Hw_SciWrite(SCI_ID0, stMdsFrame->pstMdsData->pu16DataBuff, stMdsFrame->pstMdsData->u16DataLenth);
            }
        }
    }


}
