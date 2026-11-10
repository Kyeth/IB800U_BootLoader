/**
 * @file Hw_I2cDriver.h
 * @author hanyuhang (008467)
 * @brief 
 * @version 0.1
 * @date 2025-06-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef __HW_I2CDRIVER_H
 #define __HW_I2CDRIVER_H


#include "RingBuff.h"

 #ifdef __cplusplus
    extern "C" {
#endif




#define MAX_RECV_MSGS   8       // 最大接收消息个数
#define MAX_DATA_LEN    64      // IIC最大传输长度



// I2cId与芯片ID增长顺序相关
typedef enum eSI2cId
{
    I2C_ID0     = 0x00, // 在DSP中对应I2cA，
    I2C_ID1     = 0x01, // 在DSP中对应I2cB
    I2C_ID_MAX  = 0x02,
    I2C_ID_NULL = 0xFF,
}E_I2CID;


typedef enum
{
    I2C_STATE_IDLE,
    I2C_STATE_SEND_CMD,
    I2C_STATE_WAIT_CMD_ACK,
    I2C_STATE_SEAD_DATA,
    I2C_STATE_WAIT_STOP,
    I2C_STATE_READ_DATA,
    I2C_STATE_ERROR
}E_I2C_TRANSCEIVER_STATE;

typedef struct
{
    Uint16 bTargetAddr      :10;    // 从机的设备地址
    Uint16 bRsv             :6;     // 保留
    Uint32 u32WrAddr;               // 存储介质的读写地址
    Uint32 u32Speed;
}ST_I2C_COMM_CFG;



typedef struct 
{
    Uint16 u16I2cId;
    Uint16 u16TargetAddr;
    Uint32 u32Speed;

}ST_I2C_CFG_ITEM;

typedef struct
{
    ST_I2C_CFG_ITEM *pstI2cCfgItem;
    Uint16 u16ItemNum;
}ST_I2C_CFG;


typedef struct
{
    Uint16 bDir     :1;     // 0:写操作    1:读操作
    Uint16 bAddr    :10;    // 从机的设备地址
    Uint16 bMsgId   :5;     // 消息唯一标识符
    Uint16 u16DataLength;   // 读写目标长度
    Uint32 u32WrAddr;       // 存储介质的读写地址
    Uint16 u16TargetBuff[MAX_DATA_LEN];     // 数据空间
}ST_I2C_MSG;




typedef struct
{
    ST_I2C_MSG stI2cMsg;
    Uint16 bMsgValid;
}ST_RECE_MESSAGE;



typedef struct
{
    volatile struct I2C_REGS *pstI2cRegs;
    ST_I2C_COMM_CFG stI2cCommCfg;
    ST_I2C_MSG stI2cCurrentProcMsg;
    ST_I2C_MSG stRequestMsgBuff[10];       // IIC收发命令缓存


    ST_RECE_MESSAGE RecvMsgPool[MAX_RECV_MSGS]; // 接收数据消息池
    Uint16 u16FreeStack[MAX_RECV_MSGS];         // 空闲槽位索引栈
    Uint16 u16StackTop;                         // 栈顶指针



    Uint16 u16DeviceBusy;               // IIC是否繁忙(用作互斥保护)
    int16  i16RemainData;               // 剩余发送字节数
    Uint16 u16SendDataCnt;              // 单次已发送数据长度
    Uint16 u16ReadDataCnt;              // 单次已接收数据长度
    Uint16 bIsInit      :1;                 // 1:已经初始化  0:未初始化
    Uint16 bTimeOut     :7;                 // 超时时间
    Uint16 bTimeOutCnt  :7;                 // 超时计数器
    E_I2C_TRANSCEIVER_STATE eI2cTransceiverState;   // I2C收发器状态

    // 设置FIFO大小
    ST_RING_BUFF stCmdRingBuff;
    Uint16 u16TxFifoOverFlowCount;  // 当发送命令的数据长度大于TxBuff剩余长度时，溢出计数加1，该命令抛弃
    Uint16 u16RxFifoOverFlowCount;  // 当IIC外设溢出标志置位时(RxFifo满，以及硬件Fifo也满了)，溢出计数加1

}ST_I2C_HANDLE;




/**
 * @brief 用于初始化单个I2C外设
 * 
 * @param pstI2cCfgItem 入参是一个ST_I2C_CFG_ITEM结构体对象的地址
 */
extern void Hw_SingleI2cInit(ST_I2C_CFG_ITEM *pstI2cCfgItem);



/**
 * @brief 用于初始化一组I2C，通常用于参数初始化阶段
 * 
 * @param pstI2cCfg :入参是一组ST_I2C_CFG结构体对象的集合
 */
extern void Hw_I2cInit(ST_I2C_CFG *pstI2cCfg);



/**
 * @brief 提交读/写命令请求，将请求提交到CMD缓存中
 * 
 * @param u16I2cId  :I2C的ID号
 * @param pstI2cMsg :I2C的消息对象的地址，里面需要填写读取的从机设备地址、目标内存地址、读取方向、BUFF空间等
 * @return int16    :-1 ID非法
 *                  :-2 未初始化
 *                  :-3 pstI2cMsg参数为NULL
 *                  :-4 I2C设备处于繁忙状态，正被其他线程使用
 *                  :>=0 实际写入请求命令的个数
 */
extern int16 Hw_I2cSubmitRWReuest(Uint16 u16I2cId, ST_I2C_MSG *pstI2cMsg);



/**
 * @brief 从接收FIFO中取出读出来的I2C消息
 * 
 * @param u16I2cId  :I2C的ID号
 * @param pstI2cMsg :I2C的消息对象的地址，该消息需要指定对应的消息ID
 * @param u16Size   :指定读取消息的个数
 * @return int16    :-1 ID非法
 *                  :-2 未初始化
 *                  :-3 pstI2cMsg参数为NULL
 *                  :-4 I2C设备处于繁忙状态，正被其他线程使用
 *                  :>=0 实际读取的个数
 */
extern int16 Hw_I2cRead(Uint16 u16I2cId, ST_I2C_MSG *pstI2cMsg, Uint16 u16Size);



/**
 * @brief 将指定的I2C消息对象写入到发送FIFO中
 * 
 * @param u16I2cId  :I2C的ID号
 * @param pstI2cMsg :I2C的消息对象的地址，该消息需要填充完整属性
 * @param u16Size   :指定写入消息的个数
 * @return int16    :-1 ID非法
 *                  :-2 未初始化
 *                  :-3 pstI2cMsg参数为NULL
 *                  :-4 I2C设备处于繁忙状态，正被其他线程使用
 *                  :>=0 实际写入的个数
 */
extern int16 Hw_I2cWrite(Uint16 u16I2cId, ST_I2C_MSG *pstI2cMsg, Uint16 u16Size);



/**
 * @brief I2C外设的控制接口(待实现)
 * 
 * @param u16I2cId :I2C的ID号
 * @param u16Cmd :I2C命令枚举
 * @param vParam :参数
 * @return int16 
 */
extern int16 Hw_I2cCtrl(Uint16 u16I2cId, Uint16 Cmd, void *pvParam);



/**
 * @brief I2C数据接收和发送的状态机
 * 
 * @param u16I2cId :I2C的ID号
 * @return int16    :-1 ID非法
 *                  :-2 未初始化
 *                  :0 结束
 */
extern int16 Hw_IsrI2cTransceiver(Uint16 u16I2cId);


#ifdef __cplusplus
    }
#endif



 #endif
