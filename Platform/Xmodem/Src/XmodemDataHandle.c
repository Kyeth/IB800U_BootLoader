#include "XmodemDataHandle.h"

#include "Flash280039C.h"

#ifdef __cplusplus
extern "C" {
#endif

Uint16 u16XmodemDataBuff[1024];


int ReadBlock(Uint16 u16BlockNum, Uint16 *u16Buffer)
{
    return 0;
}


int WriteBlock(Uint32 u32BlockNum, Uint16 *u16Buffer, Uint16 u16Length)
{
    int i = 0;
    Uint16 u16Value = 0;
    Uint16 u16Cnt = 0;
    Uint32 u32Length = 0;

    u32Length = u16Length;
    for (i = 0; i < u32Length; i += 2)
    {
        u16Value = u16Buffer[i] | (u16Buffer[i+1] << 8);
        u16XmodemDataBuff[u16Cnt++] = u16Value;
    }

//    if (u16BlockNum == 0 )
    {
        WriteFlashData(0x9B000U + u32BlockNum, u16XmodemDataBuff, u32Length>>1);
    }




    return u16Cnt;
}







#ifdef __cplusplus
    }
#endif
