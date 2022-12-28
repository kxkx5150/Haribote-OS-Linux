GLOBAL api_putstr1

SECTION .text

api_putstr1:    ; void apit_putstr1(char *s, int l);
    PUSH    EBX
    MOV     EDX,3
    MOV     EBX,[ESP+ 8]        ;s
    MOV     EBX,[ESP+12]        ;l
    INT     0x30
    POP     EBX
    RET
