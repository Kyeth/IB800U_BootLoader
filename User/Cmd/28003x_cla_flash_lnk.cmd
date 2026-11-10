MEMORY
{
    PAGE 0: /* PROGRAM MEMORY (FLASH) */
    BEGIN            : origin = 0x00080000, length = 0x00000002
    BOOTLOADER_VERSION  : origin = 0x00080004,length = 0x00000020   /* 起始4 Word: 软件版本 */
    BOOTLOADER_ROM     : origin = 0x00080024, length = 0x00005FD7  /* 主程序空间 (48KB - 36字节) */
    BOOTLOADER_CRC      : origin = 0x00085FFB, length = 0x00000004  /* 结束4 Word: CRC值 */

    RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM  */
    VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM  */

    RAM_ALL           : origin = 0x00008000, length = 0x00007FF8

    PAGE 1:   /* DATA MEMORY (RAM) */

    RAMM0_1          : origin = 0x00000000 length = 0x00000800
/*
    RAMLS0           : origin = 0x00008000, length = 0x00000800
    RAMLS1           : origin = 0x00008800, length = 0x00000800
    RAMLS2           : origin = 0x00009000, length = 0x00000800
    RAMLS3           : origin = 0x00009800, length = 0x00000800
    RAMLS4           : origin = 0x0000A000, length = 0x00000800
    RAMLS5           : origin = 0x0000A800, length = 0x00000800

    RAMLS6           : origin = 0x0000B000, length = 0x00000800
    RAMLS7           : origin = 0x0000B800, length = 0x00000800
*/
      RAMGS0123        : origin = 0x0000C000, length = 0x00003FF8       /* 包括了RAMGS0 RAMGS1 RAMGS2 RAMGS3 */
//      RAMGS0           : origin = 0x0000C000, length = 0x00001000
//      RAMGS1           : origin = 0x0000D000, length = 0x00001000
//      RAMGS2           : origin = 0x0000E000, length = 0x00001FF8     /* 包括了RAMGS2和RAMGS3 */


}

SECTIONS
{
//----------------------------------------------------------------------
    /*===== 程序段分配 (PAGE 0 - FLASH) =====*/
    codestart        : > BEGIN, PAGE = 0, ALIGN(4)              /* 启动代码 */
    copysections     : > BOOTLOADER_ROM, PAGE = 0, ALIGN(4)
    wddisable : > BOOTLOADER_ROM, PAGE = 0, ALIGN(4)
    version         : >  BOOTLOADER_VERSION, PAGE = 0               /* 软件版本段 */
    .crc            : >  BOOTLOADER_CRC, PAGE = 0               /* CRC校验值 */


  //.cinit          : >  BOOTLOADER_ROM, PAGE = 0, ALIGN(4)     /* C初始化表 */
  //.pinit          : >  BOOTLOADER_ROM, PAGE = 0, ALIGN(4)     /* C++初始化表 */
  //.init_array     : >  BOOTLOADER_ROM, PAGE = 0, ALIGN(4)     /* C++初始化表 */
  //.text           : >  BOOTLOADER_ROM, PAGE = 0, ALIGN(4)     /* 可执行代码 */
  //.const          : >  BOOTLOADER_ROM, PAGE = 0, ALIGN(4)     /* 常量数据 */
  //.switch         : >  BOOTLOADER_ROM, PAGE = 0, ALIGN(4)     /* Switch跳转表 */

  //.crc            : >  BOOTLOADER_CRC, PAGE = 0               /* CRC校验值 */


    GROUP
    {
        .text
        .cinit
        .pinit
       .init_array
        .const
        .switch
        .ISR.ramfunc
        .TI.ramfunc
    } LOAD = BOOTLOADER_ROM,
      RUN = RAM_ALL,
      LOAD_START(_CodeLoadStart),
      LOAD_SIZE(_CodeLoadSize),
      RUN_START(_CodeRunStart),
      ALIGN(4)
    /*===== 数据段分配 (PAGE 1 - RAM) =====*/
    .bss     : {
        . = ALIGN(4);
        *(.bss)           /* 未初始化全局变量 */
        . = ALIGN(4);
    } > RAM_ALL, PAGE = 0, ALIGN(4)

    .data    : {
        . = ALIGN(4);
        *(.data)          /* 已初始化全局变量 */
        . = ALIGN(4);
    } > RAM_ALL, PAGE = 0, ALIGN(4)

    .stack   : {
        . = ALIGN(4);
        *(.stack)         /* 系统堆栈 */
        . = ALIGN(4);
    } > RAMM0_1, PAGE = 1, ALIGN(4)

    .sysmem  : {
        . = ALIGN(4);
        . = ALIGN(4);
    } > RAM_ALL, PAGE = 0, ALIGN(4)

    /*===== 特殊段处理 =====*/
    .cio : {} > RAM_ALL , ALIGN(4)  /* C I/O缓冲区 (自动对齐) */
    .output : {} > RAM_ALL , ALIGN(4)  /* C I/O缓冲区 (自动对齐) */



//---------------------------------------------------------------------
// FLASHAPIǨ����RAM



    /* ===== 添加自动创建段的处理 ===== */
    hwbist : {} > RAM_ALL, type = NOLOAD
    hwbiststack : {} > RAM_ALL, type = NOLOAD

    .reset              : > RESET,      PAGE = 0, TYPE = DSECT
    vectors             : > VECTORS,    PAGE = 0, TYPE = DSECT
}
/*
//===========================================================================
// End of file.
//===========================================================================
*/
