#ifndef __INTERRUPTCFG_H
#define __INTERRUPTCFG_H

#ifdef __cplusplus
    extern "C" {
#endif

extern __interrupt void TaskCla_1_Isr(void);

extern void InterruptVectTableInit(void);

#ifdef __cplusplus
    }
#endif

#endif
