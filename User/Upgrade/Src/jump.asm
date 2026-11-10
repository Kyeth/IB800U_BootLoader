

jump_to_app:
    LB  0x86000          ; 汇编器会帮你拼 06 3F 60 00 ...
    NOP                  ; 流水线填充
.end
