[BITS 32]
    MOV AL,'h'
    CALL 2*8:0x1894
    MOV AL,'e'
    CALL 2*8:0x1894
    MOV AL,'l'
    CALL 2*8:0x1894
    MOV AL,'l'
    CALL 2*8:0x1894
    MOV AL,'o'
    CALL 2*8:0x1894
    RETF
fin:
    HLT
    JMP fin