[BITS 32]
    MOV     EAX,1*8     ; OS用のセグメント番号
    MOV     DS,AX       ; これをDSに入れちゃう
    MOV     BYTE [0x102600],0
    RETF