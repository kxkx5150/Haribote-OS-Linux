[BITS 32]
    MOV AL,'A'
    CALL 0x1894
fin:
    HLT
    JMP fin