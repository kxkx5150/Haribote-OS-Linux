; nasmfunc.asm
; TAB=4

section .text
    GLOBAL  io_hlt, io_cli, io_sti, io_stihlt
    GLOBAL  io_in8, io_in16, io_in32
    GLOBAL  io_out8, io_out16, io_out32
    GLOBAL  io_load_eflags, io_store_eflags
    GLOBAL  load_gdtr, load_idtr
    GLOBAL  asm_inthandler20, asm_inthandler21, asm_inthandler27, asm_inthandler2c
    GLOBAL  store_cr0, load_cr0, memtest_sub
    GLOBAL  load_tr, farjmp
    GLOBAL  asm_cons_putchar
    EXTERN  inthandler20, inthandler21, inthandler27, inthandler2c
    EXTERN  cons_putchar

io_hlt:        ; void io_hlt(void);
    HLT
    RET

io_cli:
    CLI
    RET

io_sti:
    STI
    RET

io_stihlt:
    STI
    HLT
    RET

io_in8:
    MOV     EDX,[ESP+4]
    MOV     EAX,0
    IN      AL,DX
    RET

io_in16:
    MOV     EDX,[ESP+4]
    MOV     EAX,0
    IN      AX,DX
    RET

io_in32:
    MOV     EDX,[ESP+4]
    IN      EAX,DX
    RET

io_out8:
    MOV     EDX,[ESP+4]
    MOV     AL,[ESP+8]
    OUT     DX,AL
    RET

io_out16:
    MOV     EDX,[ESP+4]
    MOV     EAX,[ESP+8]
    OUT     DX,AX
    RET

io_out32:
    MOV     EDX,[ESP+4]
    MOV     AL,[ESP+8]
    OUT     DX,EAX
    RET

io_load_eflags:
    PUSHFD
    POP     EAX
    RET

io_store_eflags:
    MOV     EAX,[ESP+4]
    PUSH    EAX
    POPFD
    RET

load_gdtr:  ; void load_gdtr(int limit, int addr);
    MOV     AX,[ESP+4]      ; limit
    MOV     [ESP+6],AX
    LGDT    [ESP+6]
    RET

load_idtr:  ; void load_idtr(int limit, int addr);
    MOV     AX,[ESP+4]      ; limit
    MOV     [ESP+6],AX
    LIDT    [ESP+6]
    RET



asm_inthandler21:
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX,ESP
    PUSH    EAX
    MOV     AX,SS
    MOV     DS,AX
    MOV     ES,AX
    CALL    inthandler21
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD

asm_inthandler27:
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX,ESP
    PUSH    EAX
    MOV     AX,SS
    MOV     DS,AX
    MOV     ES,AX
    CALL    inthandler27
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD

asm_inthandler2c:
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX,ESP
    PUSH    EAX
    MOV     AX,SS
    MOV     DS,AX
    MOV     ES,AX
    CALL    inthandler2c
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD

load_cr0:       ;int load_cr0(void);
    MOV     EAX,CR0
    RET

store_cr0:       ;void store_cr0(int cr0);
    MOV     EAX,[ESP+4]
    MOV     CR0,EAX
    RET

memtest_sub:    ; unsigned int memtest_sub(unsigned int start, unsigned int end)
    PUSH    EDI                     ;   (EBX, ESI, EDI も使いたいため)
    PUSH    ESI
    PUSH    EBX
    MOV     ESI,0xaa55aa55          ; pat0 = 0xaa55aa55;
    MOV     EDI,0x55aa55aa          ; pat1 = 0x55aa55aa;
    MOV     EAX,[ESP+12+4]          ; i = start;
mts_loop:
    MOV     EBX,EAX
    ADD     EBX,0xffc               ; p = (unsigned int *) (i + 0xffc)
    MOV     EDX,[EBX]               ; old = *p
    MOV     [EBX],ESI               ; *p = pat0
    XOR     DWORD [EBX],0xffffffff  ; *p ^= 0xffffffff;
    CMP     EDI,[EBX]               ; if (*p != pat1) goto fin;
    JNE     mts_fin
    XOR     DWORD [EBX],0xffffffff  ; *p ^= 0xffffffff;
    CMP     ESI,[EBX]               ; if (*p != pat0) goto fin;
    JNE     mts_fin
    MOV     [EBX],EDX               ; *p = old;
    ADD     EAX,0x1000              ; i += 0x1000;
    CMP     EAX,[ESP+12+8]          ; if (i <= end) goto mts_loop;
    JBE     mts_loop
    POP     EBX
    POP     ESI
    POP     EDI
    RET
mts_fin:
    MOV     [EBX],EDX               ; *p = old;
    POP     EBX
    POP     ESI
    POP     EDI
    RET

asm_inthandler20:
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX,ESP
    PUSH    EAX
    MOV     AX,SS
    MOV     DS,AX
    MOV     ES,AX
    CALL    inthandler20
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD

load_tr:    ; void load_tr(int tr);
    LTR     [ESP+4]     ;tr
    RET

farjmp:     ; void farjmp(int eip, int cs);
    JMP     FAR [ESP+4]     ; eip, cs
    RET

asm_cons_putchar:
    PUSH    1
    AND     EAX,0xff    ; AHやEAXの上位を0にして、EAXに文字コードが入った状態にする
    PUSH    EAX
    PUSH    DWORD [0x0fec] ; メモリの内容を読み込んでその値をPUSHする
    CALL    cons_putchar
    ADD     ESP,12      ; スタックに積んだデーをを捨てる
    RETF