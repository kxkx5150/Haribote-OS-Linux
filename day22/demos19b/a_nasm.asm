;a_nasm.asm
GLOBAL api_putchar
GLOBAL api_end

SECTION .text

api_putchar:
    MOV EDX,1
    MOV AL, [ESP+4]     ; c
    INT 0x40
    RET

api_end:    ; void api_end(void);
    MOV EDX,4
    INT 0x40