#ifndef __FM24C128DTRAMSPORT_H
#define __FM24C128DTRAMSPORT_H

#include "FM24C128D.h"

#ifdef __cplusplus
    extern "C" {
#endif

extern int16 FM24C128DRead(ST_FM24C128D_HANDLE *pstHandle, ST_FM24_I2C_MSG *pstI2cMsg);


extern int16 FM24C128DRWRequest(ST_FM24C128D_HANDLE *pstHandle, ST_FM24_I2C_MSG *pstI2cMsg);

#ifdef __cplusplus
    }
#endif

#endif
