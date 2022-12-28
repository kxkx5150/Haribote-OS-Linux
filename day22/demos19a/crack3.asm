[BITS 32]
    MOV    AL,0x34
    OUT    0x43,AL
    MOV    AL,0xff
    OUT    0x30,AL
    MOV    AL,0xff
    OUT    0x40,AL

; io_out(PIT_CTRL, 0x34);
; io_out(PIT_CNT0, 0xff);
; io_out(PIT_CNT0, 0xff);

    MOV    EDX,4
    INT    0x40