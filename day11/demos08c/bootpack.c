#include "bootpack.h"

void HariMain(void) {
  struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
  char s[40], mcursor[256], keybuf[32], mousebuf[128];
  int mx, my, i;
  struct MOUSE_DEC mdec;
  unsigned int memtotal;
  struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
  struct SHTCTL *shtctl;
  struct SHEET *sht_back, *sht_mouse;
  unsigned char *buf_back, buf_mouse[256];

  init_gdtidt();
  init_pic();
  io_sti();  // IDT/PICの初期化が終わったのでCPUの割り込み禁止を解除
  fifo8_init(&keyfifo, 32, keybuf);
  fifo8_init(&mousefifo, 128, mousebuf);
  io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
  io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */

  init_keyboard();
  enable_mouse(&mdec);
  memtotal = memtest(0x00400000, 0xbfffffff);
  memman_init(memman);
  memman_free(memman, 0x00001000, 0x009e000);  // 0x00001000 - 0x0009efff
  memman_free(memman, 0x00400000, memtotal - 0x00400000);

  init_palette();
  shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
  sht_back = sheet_alloc(shtctl);
  sht_mouse = sheet_alloc(shtctl);
  buf_back =
      (unsigned char *)memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
  sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny,
               -1);                                    // 透明色なし
  sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);      // 透明ば番号は99
  init_screen8(buf_back, binfo->scrnx, binfo->scrny);  // TODO: screen8?
  init_mouse_cursor8(buf_mouse, 99);
  sheet_slide(sht_back, 0, 0);
  mx = (binfo->scrnx - 16) / 2;  // 画面中央になるように座標計算
  my = (binfo->scrny - 28 - 16) / 2;
  sheet_slide(sht_mouse, mx, my);
  sheet_updown(sht_back, 0);
  sheet_updown(sht_mouse, 1);
  mysprintf(s, "(%d, %d)", mx, my);
  putfonts8_asc(buf_back, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
  mysprintf(s, "memory %dMB   free: %dKB", memtotal / (1024 * 1024),
            memman_total(memman) / 1024);
  putfonts8_asc(buf_back, binfo->scrnx, 0, 32, COL8_FFFFFF, s);
  sheet_refresh(sht_back, 0, 0, binfo->scrnx, 48);

  for (;;) {
    io_cli();
    if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
      io_stihlt();
    } else {
      if (fifo8_status(&keyfifo) != 0) {
        i = fifo8_get(&keyfifo);
        io_sti();
        mysprintf(s, "%X", i);
        boxfill8(buf_back, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
        putfonts8_asc(buf_back, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
        sheet_refresh(sht_back, 0, 16, 16, 32);
      } else if (fifo8_status(&mousefifo) != 0) {
        i = fifo8_get(&mousefifo);
        io_sti();
        if (mouse_decode(&mdec, i) != 0) {
          // データが3バイト揃ったので表示
          mysprintf(s, "[lcr %d %d]", mdec.x, mdec.y);
          // クリックしたボタンを大文字で表示
          if ((mdec.btn & 0x01) != 0) {
            s[1] = 'L';
          }
          if ((mdec.btn & 0x02) != 0) {
            s[3] = 'R';
          }
          if ((mdec.btn & 0x04) != 0) {
            s[2] = 'C';
          }
          boxfill8(buf_back, binfo->scrnx, COL8_008484, 32, 16, 32 + 15 * 8 - 1,
                   31);
          putfonts8_asc(buf_back, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
          sheet_refresh(sht_back, 32, 16, 32 + 15 * 8, 32);

          // マウスカーソルの移動
          // 移動量を加算
          mx += mdec.x;
          my += mdec.y;

          // 左/上端にきたらx/y座標を維持
          if (mx < 0) {
            mx = 0;
          }
          if (my < 0) {
            my = 0;
          }
          // 右/下端（からマウスのサイズを引いた位置）にきたらx/y座標を維持
          if (mx > binfo->scrnx - 1) {
            mx = binfo->scrnx - 1;
          }
          if (my > binfo->scrny - 1) {
            my = binfo->scrny - 1;
          }
          mysprintf(s, "(%d, %d)", mx, my);
          boxfill8(buf_back, binfo->scrnx, COL8_008484, 0, 0, 79,
                   15);  // 座標消す
          putfonts8_asc(buf_back, binfo->scrnx, 0, 0, COL8_FFFFFF,
                        s);  // 座標書く
          sheet_refresh(sht_back, 0, 0, 80, 16);
          sheet_slide(sht_mouse, mx, my);  // sheet_refreshを含む
        }
      }
    }
  }
}
