/*
 * DrvCfg_Ep1300.c
 *
 *  Created on: 2025年5月16日
 *      Author: 17537
 */

#include "DrvCfg_IB800.h"
#include "InterruptCfg.h"


#ifdef __cplusplus
    extern "C" {
#endif

/*
 *  所有用到的IO口初始化配置均在此处
 */
ST_GPIO_CFG_ITEM stGpioCfgItem[] =
{
     {6,    GPIO,       DIR_DO, 1, GPIO_OD_DISABEL},        // LED1
     {34,   GPIO,       DIR_DO, 1, GPIO_OD_DISABEL},        // LED2

     {47,   GPIO,       DIR_DI, 1, GPIO_OD_DISABEL},        // 支路1过流保护
     {49,   GPIO,       DIR_DI, 1, GPIO_OD_DISABEL},        // 支路2过流保护
     {53,   GPIO,       DIR_DI, 1, GPIO_OD_DISABEL},        // 输入过流保护
     
     {23,   GPIO,       DIR_DO, 0, GPIO_OD_DISABEL},        // Relay
     {16,   GPIO,       DIR_DO, 0, GPIO_OD_DISABEL},        // Fan
     {41,   GPIO,       DIR_DO, 0, GPIO_OD_DISABEL},        // 预充电阻

     {0,    PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM1A
     {1,    PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM1B

     {2,    PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM2A
     {3,    PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM2B

     {4,    PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM3A
     {5,    PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM3B

     {22,   PERIPH11,   DIR_DO, 0, GPIO_OD_DISABEL},          // PWM4A
     {7,    PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM4B

     {8,    PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM5A
     {9,    PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM5B

     {10,   PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM6A
     {11,   PERIPH1,    DIR_DO, 0, GPIO_OD_DISABEL},          // PWM6B

     {29,   PERIPH1,    DIR_DO, 1, GPIO_OD_DISABEL},          // SCIA_TX
     {28,   PERIPH1,    DIR_DI, 1, GPIO_OD_DISABEL},          // SCIA_RX    SCI不能使能开漏模式

     {12,   PERIPH5,    DIR_DO, 1, GPIO_OD_DISABEL},          // SCIB_TX
     {13,   PERIPH5,    DIR_DI, 1, GPIO_OD_DISABEL},          // SCIB_RX    SCI不能使能开漏模式

     {31,   PERIPH1,    DIR_DO, 1, GPIO_OD_DISABEL},          // CANA_TX
     {30,   PERIPH1,    DIR_DI, 1, GPIO_OD_DISABEL},          // CANA_RX    CAN不能使能开漏模式

//     {12,   PERIPH5,    DIR_DO, 1, GPIO_OD_DISABEL},          // M_CANB_TX
//     {13,   PERIPH5,    DIR_DI, 1, GPIO_OD_DISABEL},          // M_CANB_RX    M_CAN不能使能开漏模式

     {27,   PERIPH9,    DIR_DO, 1, GPIO_OD_DISABEL},          // I2C_SCL
     {26,   PERIPH9,    DIR_DI, 1, GPIO_OD_DISABEL},          // I2C_SDA

     {25,   GPIO,    DIR_DO, 1, GPIO_OD_DISABEL},           // I2C_WP

     {60,   PERIPH3,    DIR_DO, 1, GPIO_OD_DISABEL},          // 调试MCANA_TX
     {61,   PERIPH3,    DIR_DI, 1, GPIO_OD_DISABEL},          // 调试MCANA_RX    CAN不能使能开漏模式
};

ST_GPIO_CFG stGpioCfg = {stGpioCfgItem, sizeof(stGpioCfgItem)/sizeof(stGpioCfgItem[0])};


/*
 * SCI外设初始化配置
 */
ST_SCI_CFG_ITEM stSciCfgItem[] =
{
     {SCI_ID0, 38400, DATABITS_8, STOPBITS_1, DISABLE_PARITY, 0, 0, 0, 0},
     {SCI_ID1, 38400, DATABITS_8, STOPBITS_1, DISABLE_PARITY, 0, 0, 0, 0},
};
ST_SCI_CFG stSciCfg = {stSciCfgItem, sizeof(stSciCfgItem)/sizeof(stSciCfgItem[0])};





#ifdef __cplusplus
    }
#endif
