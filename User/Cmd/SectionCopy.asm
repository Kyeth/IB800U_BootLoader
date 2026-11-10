    .ref _c_int00
    .global copy_sections

    .global _CodeLoadStart, _CodeLoadSize, _CodeRunStart

    .sect "copysections"

copy_sections:

    MOVL XAR5, #_CodeLoadSize
    MOVL ACC, @XAR5
    MOVL XAR6, #_CodeLoadStart
    MOVL XAR7, #_CodeRunStart
    LCR copy


    LB _c_int00

copy:
    B return,EQ
    SUBB ACC,#1
    RPT AL
    || PWRITE *XAR7, *XAR6++

return:
    LRETR


    .end
