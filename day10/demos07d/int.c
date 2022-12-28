#include"bootpack.h"

void init_pic(void)
// PICの初期化
{
    io_out8(PIC0_IMR,   0xff); // すべての割り込みを受け付けない
    io_out8(PIC1_IMR,   0xff); // すべての割り込みを受け付けない

    io_out8(PIC0_ICW1,  0x11); // エッジトリガーモード
    io_out8(PIC0_ICW2,  0x20); // IRQ0-7は，INT20-27で受ける
    io_out8(PIC0_ICW3,  1 << 2); // PIC1はIRQ2にて接続
    io_out8(PIC0_ICW4,  0x01); // ノンバッファモード

    io_out8(PIC1_ICW1,  0x11);
    io_out8(PIC1_ICW2,  0x28);
    io_out8(PIC1_ICW3,  2);
    io_out8(PIC1_ICW4,  0x01);

    io_out8(PIC0_IMR,   0xfb); // 11111011 PIC1以外はすべて禁止
    io_out8(PIC1_IMR,   0xff); // 11111111 すべての割り込みを受け付け

    return;
}

// struct FIFO8 keyfifo;
// struct FIFO8 mousefifo;

/* PIC0からの不完全割り込み対策 */
/* Athlon64X2機などではチップセットの都合によりPICの初期化時にこの割り込みが1度だけおこる */
/* この割り込み処理関数は、その割り込みに対して何もしないでやり過ごす */
/* なぜ何もしなくていいの？
	→  この割り込みはPIC初期化時の電気的なノイズによって発生したものなので、
		まじめに何か処理してやる必要がない。									*/
void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67); /* IRQ-07受付完了をPICに通知(7-1参照) */
	return;
}
