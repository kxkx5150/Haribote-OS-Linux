;a_nasm.asm
GLOBAL api_putchar
GLOBAL api_end
GLOBAL api_putstr0

SECTION .text

api_putchar:
    MOV EDX,1
    MOV AL, [ESP+4]     ; c
    INT 0x40
    RET

api_end:    ; void api_end(void);
    MOV EDX,4
    INT 0x40

api_putstr0: ; void api_putstr0(char *s);
    PUSH EBX
    MOV EDX,2
    MOV EBX, [ESP+8]    ; s
    INT 0x40
    POP EBX
    RET
