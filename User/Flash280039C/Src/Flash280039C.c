//#include "Flash280039C.h"
//
//
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//ST_DSP280039FLASH stDsp280039Flash;
//
//__attribute__((aligned(8))) ST_UPGRADE_CFG stUpgradeCfg;                  // 需要128bit对齐
//
//__attribute__((aligned(8))) Uint16 u16DataBuffPing[DATA_BUFF_SIZE];       // 需要128bit对齐
//__attribute__((aligned(8))) Uint16 u16DataBuffPang[DATA_BUFF_SIZE];       // 需要128bit对齐
//
//void Flash280039CInit(void)
//{
//    Dsp280039FlashInit(&stDsp280039Flash, u16DataBuffPing, u16DataBuffPang, Bone_Sector11_start, &stUpgradeCfg);
//}
//
//
//
//void TaskFlash280039C(void)
//{
//    Dsp280039FlashHandle(&stDsp280039Flash);
//}
//
//
//#ifdef __cplusplus
//}
//#endif
