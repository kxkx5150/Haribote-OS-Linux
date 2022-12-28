#include "bootpack.h"

void console_task(struct SHEET *sheet, unsigned int memtotal)
{
  struct TASK *task = task_now();
  struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
  int i, *fat = (int *)memman_alloc_4k(memman, 4 * 2880);
  struct CONSOLE cons;
  char cmdline[30];
  cons.sht = sheet;
  cons.cur_x = 8;
  cons.cur_y = 28;
  cons.cur_c = -1;
  task->cons = &cons; // 直す前: *((int *)0x0fec) = (int)&cons;

  cons.timer = timer_alloc();
  timer_init(cons.timer, &task->fifo, 1);
  timer_settime(cons.timer, 50);
  file_readfat(fat, (unsigned char *)(ADR_DISKING + 0x000200));

  // プロンプト表示
  cons_putchar(&cons, '>', 1);

  for (;;)
  {
    io_cli();
    if (fifo32_status(&task->fifo) == 0)
    {
      task_sleep(task);
      io_sti();
    }
    else
    {
      i = fifo32_get(&task->fifo);
      io_sti();
      if (i <= 1)
      { // カーソル用タイマー
        if (i != 0)
        {
          timer_init(cons.timer, &task->fifo, 0); // 次は0を
          if (cons.cur_c >= 0)
          {
            cons.cur_c = COL8_FFFFFF;
          }
        }
        else
        {
          timer_init(cons.timer, &task->fifo, 1); // 次は1を
          if (cons.cur_c >= 0)
          {
            cons.cur_c = COL8_000000;
          }
        }
        timer_settime(cons.timer, 50);
      }
      if (i == 2)
      { //カーソルON
        cons.cur_c = COL8_FFFFFF;
      }
      if (i == 3)
      { //カーソルOFF
        boxfill8(sheet->buf, sheet->bxsize, COL8_000000, cons.cur_x, cons.cur_y,
                 cons.cur_x + 7, cons.cur_y + 15);
        cons.cur_c = -1;
      }
      if (256 <= i && i <= 511)
      { // キーボードデータ（タスクA経由）
        if (i == 8 + 256)
        {
          // バックスペース
          if (cons.cur_x > 16)
          {
            // カーソルをスペースで消してから、カーソルを一つ戻す
            cons_putchar(&cons, ' ', 0);
            cons.cur_x -= 8;
          }
        }
        else if (i == 10 + 256)
        {
          // Enter
          // カーソルをスペースで消す
          cons_putchar(&cons, ' ', 0);
          cmdline[cons.cur_x / 8 - 2] = 0;
          cons_newline(&cons);
          cons_runcmd(cmdline, &cons, fat, memtotal); // コマンド実行

          // プロンプト表示
          cons_putchar(&cons, '>', 1);
        }
        else
        { // 一般文字
          if (cons.cur_x < 240)
          {
            // 一文字表示してから、カーソルを一つ埋める
            cmdline[cons.cur_x / 8 - 2] = i - 256;
            cons_putchar(&cons, i - 256, 1);
          }
        }
      }
      // カーソル再表示
      if (cons.cur_c >= 0)
      {
        boxfill8(sheet->buf, sheet->bxsize, cons.cur_c, cons.cur_x, cons.cur_y,
                 cons.cur_x + 7, cons.cur_y + 15);
      }
      sheet_refresh(sheet, cons.cur_x, cons.cur_x + 8, cons.cur_x + 8,
                    cons.cur_y + 16);
    }
  }
}

void cons_putchar(struct CONSOLE *cons, int chr, char move)
{
  char s[2];
  s[0] = chr;
  s[1] = 0;
  if (s[0] == 0x09)
  { // タブ
    for (;;)
    {
      putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, COL8_FFFFFF,
                        COL8_000000, " ", 1);
      cons->cur_x += 8;
      if (cons->cur_x == 8 + 240)
      {
        cons_newline(cons);
      }
      if (((cons->cur_x - 8) & 0x1f) == 0)
      {
        break; // 32で割り切れたらbreak
      }
    }
  }
  else if (s[0] == 0x0a)
  { //改行
    cons_newline(cons);
  }
  else if (s[0] == 0x0d)
  { //復帰
    // なにもしない
  }
  else
  { // 普通の文字
    putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, COL8_FFFFFF,
                      COL8_000000, s, 1);
    if (move != 0)
    {
      // moveが0のときはカーソルを勧めない
      cons->cur_x += 8;
      if (cons->cur_x == 8 + 240)
      {
        cons_newline(cons);
      }
    }
  }
  return;
}

void cons_newline(struct CONSOLE *cons)
{
  int x, y;
  struct SHEET *sheet = cons->sht;
  if (cons->cur_y < 28 + 112)
  {
    cons->cur_y += 16; // 次の行へ
  }
  else
  {
    // スクロール
    for (y = 28; y < 28 + 112; y++)
    {
      for (x = 8; x < 8 + 240; x++)
      {
        sheet->buf[x + y * sheet->bxsize] =
            sheet->buf[x + (y + 16) * sheet->bxsize];
      }
    }
    for (y = 28 + 112; y < 28 + 128; y++)
    {
      for (x = 8; x < 8 + 240; x++)
      {
        sheet->buf[x + y * sheet->bxsize] = COL8_000000;
      }
    }
    sheet_refresh(sheet, 8, 28, 8 + 240, 28 + 128);
  }
  cons->cur_x = 8;
  return;
}

void cons_runcmd(char *cmdline, struct CONSOLE *cons, int *fat,
                 unsigned int memtotal)
{
  if (mystrcmp(cmdline, "mem") == 0)
  {
    cmd_mem(cons, memtotal);
  }
  else if (mystrcmp(cmdline, "cls") == 0)
  {
    cmd_cls(cons);
  }
  else if (mystrcmp(cmdline, "dir") == 0)
  {
    cmd_dir(cons);
  }
  else if (mystrncmp(cmdline, "type ", 5) == 0)
  {
    cmd_type(cons, fat, cmdline);
  }
  else if (cmdline[0] != 0)
  {
    if (cmd_app(cons, fat, cmdline) == 0)
    {
      // コマンドではなく、アプリでもなく、さらに改行でもない
      cons_putstr0(cons, "Bad command.\n\n");
    }
  }
  return;
}

void cmd_mem(struct CONSOLE *cons, unsigned int memtotal)
{
  // memコマンド
  struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
  char s[60];
  mysprintf(s, "total   %dMB\nfree %dKB\n\n", memtotal / (1024 * 1024),
            memman_total(memman) / 1024);
  cons_putstr0(cons, s);
  return;
}

void cmd_cls(struct CONSOLE *cons)
{
  // clsコマンド
  int x, y;
  struct SHEET *sheet = cons->sht;
  for (y = 28; y < 28 + 128; y++)
  {
    for (x = 8; x < 8 + 240; x++)
    {
      sheet->buf[x + y * sheet->bxsize] = COL8_000000;
    }
  }
  sheet_refresh(sheet, 8, 28, 8 + 240, 28 + 128);
  cons->cur_y = 28;
  return;
}

void cmd_dir(struct CONSOLE *cons)
{
  // dirコマンド
  struct FILEINFO *finfo = (struct FILEINFO *)(ADR_DISKING + 0x002600);
  int i, j;
  char s[30];

  for (i = 0; i < 224; i++)
  {
    if (finfo[i].name[0] == 0x00)
    {
      break;
    }
    if (finfo[i].name[0] != 0xe5)
    {
      if ((finfo[i].type & 0x18) == 0)
      {
        mysprintf(s, "filename.ext   %d\n", finfo[i].size);
        for (j = 0; j < 8; j++)
        {
          s[j] = finfo[i].name[j];
        }
        s[9] = finfo[i].ext[0];
        s[10] = finfo[i].ext[1];
        s[11] = finfo[i].ext[2];
        cons_putstr0(cons, s);
      }
    }
  }
  cons_newline(cons);
  return;
}

void cmd_type(struct CONSOLE *cons, int *fat, char *cmdline)
{
  // typeコマンド
  struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
  struct FILEINFO *finfo = file_search(
      cmdline + 5, (struct FILEINFO *)(ADR_DISKING + 0x002600), 224);
  char *p;
  int i;
  if (finfo != 0)
  {
    // ファイルが見つかった場合
    p = (char *)memman_alloc_4k(memman, finfo->size);
    file_loadfile(finfo->clustno, finfo->size, p, fat,
                  (char *)(ADR_DISKING + 0x003e00));
    cons_putstr1(cons, p, finfo->size);
    memman_free_4k(memman, (int)p, finfo->size);
  }
  else
  {
    // ファイルが見つからなかった場合
    cons_putstr0(cons, "File not found.\n");
  }
  cons_newline(cons);
  return;
}

int cmd_app(struct CONSOLE *cons, int *fat, char *cmdline)
{
  struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
  struct FILEINFO *finfo;
  struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)ADR_GDT;
  char name[18], *p, *q;
  struct TASK *task = task_now();
  struct SHTCTL *shtctl;
  struct SHEET *sht;
  int i;
  int segsiz, datsiz, esp, dathrb;

  // コマンドラインからファイル名を生成
  for (i = 0; i < 13; i++)
  {
    if (cmdline[i] <= ' ')
    {
      break;
    }
    name[i] = cmdline[i];
  }
  name[i] = 0; // とりあえずファイル名のうしろを0とする

  // ファイルを探す
  finfo = file_search(name, (struct FILEINFO *)(ADR_DISKING + 0x002600), 224);
  if (finfo == 0 && name[i - 1] != '.')
  {
    // 見つからなければ後ろに、.BINをつけてもう一度探してみる
    name[i] = '.';
    name[i + 1] = 'H';
    name[i + 2] = 'R';
    name[i + 3] = 'B';
    name[i + 4] = 0;
    finfo = file_search(name, (struct FILEINFO *)(ADR_DISKING + 0x002600), 224);
  }

  if (finfo != 0)
  {
    // ファイルが見つかった場合
    p = (char *)memman_alloc_4k(memman, finfo->size);
    file_loadfile(finfo->clustno, finfo->size, p, fat,
                  (char *)(ADR_DISKING + 0x003e00));
    if (finfo->size >= 36 && mystrncmp(p + 4, "Hari", 4) == 0 && *p == 0x00)
    {
      segsiz = *((int *)(p + 0x0000));
      esp = *((int *)(p + 0x000c));
      datsiz = *((int *)(p + 0x0010));
      dathrb = *((int *)(p + 0x0014));
      q = (char *)memman_alloc_4k(memman, segsiz);
      task->ds_base = (int)q; // 直す前: *((int *)0xfe8) = (int)q;
      set_segmdesc(gdt + task->sel / 8 + 1000, finfo->size - 1, (int)p, AR_CODE32_ER + 0x60);
      set_segmdesc(gdt + task->sel / 8 + 2000, segsiz - 1, (int)q, AR_DATA32_RW + 0x60);
      for (i = 0; i < datsiz; i++)
      {
        q[esp + i] = p[dathrb + i];
      }
      start_app(0x1b, task->sel + 1000 * 8, esp, task->sel + 2000 * 8,
                &(task->tss.esp0)); // 0x1b (HariMainの番地)
      shtctl = (struct SHTCTL *)*((int *)0x0fe4);
      for (i = 0; i < MAX_SHEETS; i++)
      {
        sht = &(shtctl->sheets0[i]);
        if ((sht->flags & 0x11) == 0x11 && sht->task == task)
        {
          // アプリが開きっぱなしにした下敷きを発見
          sheet_free(sht); //閉じる
        }
      }
      timer_cancelall(&task->fifo);
      memman_free_4k(memman, (int)q, segsiz);
    }
    else
    {
      cons_putstr0(cons, ".hrb file format error,\n");
    }
    memman_free_4k(memman, (int)p, finfo->size);
    cons_newline(cons);
    return 1;
  }
  // ファイルが見つからなかった場合
  return 0;
}

void cons_putstr0(struct CONSOLE *cons, char *s)
{
  for (; *s != 0; s++)
  {
    cons_putchar(cons, *s, 1);
  }
  return;
}

void cons_putstr1(struct CONSOLE *cons, char *s, int l)
{
  int i;
  for (i = 0; i < l; i++)
  {
    cons_putchar(cons, s[i], 1);
  }
  return;
}

void *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx,
              int eax)
{
  struct TASK *task = task_now();
  int ds_base = task->ds_base;
  struct CONSOLE *cons = task->cons;
  struct SHTCTL *shtctl = (struct SHTCTL *)*((int *)0x0fe4);
  struct SHEET *sht;
  int i;
  int *reg = &eax + 1; // eaxの次の番地
                       // 保存のためにPUSHADを強引に書き換える
                       // reg[0]: EDI, reg[1]: ESI, reg[2]: ESP, reg[3]: ESP
                       // reg[4]: EBX, reg[5]: EDX, reg[6]: ECX, reg[7]: EAX
  if (edx == 1)
  {
    cons_putchar(cons, eax & 0xff, 1);
  }
  else if (edx == 2)
  {
    cons_putstr0(cons, (char *)ebx + ds_base);
  }
  else if (edx == 3)
  {
    cons_putstr1(cons, (char *)ebx + ds_base, ecx);
  }
  else if (edx == 4)
  {
    return &(task->tss.esp0);
  }
  else if (edx == 5)
  {
    // Window表示API
    sht = sheet_alloc(shtctl);
    sht->task = task;
    sht->flags |= 0x10;
    sheet_setbuf(sht, (char *)ebx + ds_base, esi, edi, eax);
    make_window8((char *)ebx + ds_base, esi, edi, (char *)ecx + ds_base, 0);
    sheet_slide(sht, ((shtctl->xsize - esi) / 2) & ~3, (shtctl->ysize - edi) / 2);
    sheet_updown(sht, shtctl->top); // 今のマウスと同じ高さになるように指定（マウスはこの上になる）
    reg[7] = (int)sht;
  }
  else if (edx == 6)
  {                                           // ウィンドウへ文字を表示させるAPIの呼び出し
    sht = (struct SHEET *)(ebx & 0xfffffffe); // 2の倍数になるように切り捨てる
    putfonts8_asc(sht->buf, sht->bxsize, esi, edi, eax, (char *)ebp + ds_base);
    if ((ebx & 1) == 0)
    {
      sheet_refresh(sht, esi, edi, esi + ecx * 8, edi + 16);
    }
  }
  else if (edx == 7)
  {                                           // ウィンドウに四角を描くAPIの呼び出し
    sht = (struct SHEET *)(ebx & 0xfffffffe); // 2の倍数になるように切り捨てる
    boxfill8(sht->buf, sht->bxsize, ebp, eax, ecx, esi, edi);
    if ((ebx & 1) == 0)
    {
      sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
    }
  }
  else if (edx == 8)
  {
    // memmanの初期化
    memman_init((struct MEMMAN *)(ebx + ds_base));
    ecx &= 0xfffffff0; // 16バイト単位に
    memman_free((struct MEMMAN *)(ebx + ds_base), eax, ecx);
  }
  else if (edx == 9)
  {
    // malloc
    ecx = (ecx + 0x0f) & 0xfffffff0; // 16バイト単位に繰り上げ
    reg[7] = memman_alloc((struct MEMMAN *)(ebx + ds_base), ecx);
  }
  else if (edx == 10)
  {
    // free
    ecx = (ecx + 0x0f) & 0xfffffff0; // 16バイト単位に繰り上げ
    memman_free((struct MEMMAN *)(ebx + ds_base), eax, ecx);
  }
  else if (edx == 11)
  {
    // 点を描く
    sht = (struct SHEET *)(ebx & 0xfffffffe); // 2の倍数になるように切り捨てる
    sht->buf[sht->bxsize * edi + esi] = eax;
    if ((ebx & 1) == 0)
    {
      sheet_refresh(sht, esi, edi, esi + 1, edi + 1);
    }
  }
  else if (edx == 12)
  {
    // 画面リフレッシュ
    sht = (struct SHEET *)ebx;
    sheet_refresh(sht, eax, ecx, esi, edi);
  }
  else if (edx == 13)
  {
    // ウィンドウに線を引く
    sht = (struct SHEET *)(ebx & 0xfffffffe); // 2の倍数になるように切り捨てる
    hrb_api_linewin(sht, eax, ecx, esi, edi, ebp);
    if ((ebx & 1) == 0)
    {
      sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
    }
  }
  else if (edx == 14)
  {
    // ウィンドウを閉じる
    sheet_free((struct SHEET *)ebx);
  }
  else if (edx == 15)
  {
    // キー入力
    for (;;)
    {
      io_cli();
      if (fifo32_status(&task->fifo) == 0)
      {
        if (eax != 0)
        {
          task_sleep(task); // FIFOがからなので寝て待つ
        }
        else
        {
          io_sti();
          reg[7] = -1;
          return 0;
        }
      }
      i = fifo32_get(&task->fifo);
      io_sti();
      if (i <= 1)
      { // カーソル用タイマ
        // アプリ実行中はカーソルが出ないので、いつも次の表示用の1を注文しておく
        timer_init(cons->timer, &task->fifo, 1); // 次は1を
        timer_settime(cons->timer, 50);
      }
      if (i == 2)
      { // カーソルON
        cons->cur_c = COL8_FFFFFF;
      }
      if (i == 3)
      { // カーソルOFF
        cons->cur_c = -1;
      }
      if (i >= 256)
      { //キーボードデータ（タスクA経由）など
        reg[7] = i - 256;
        return 0;
      }
    }
  }
  else if (edx == 16)
  {
    reg[7] = (int)timer_alloc();
    ((struct TIMER *)reg[7])->flags2 = 1; // 自動キャンセル有効
  }
  else if (edx == 17)
  {
    timer_init((struct TIMER *)ebx, &task->fifo, eax + 256);
  }
  else if (edx == 18)
  {
    timer_settime((struct TIMER *)ebx, eax);
  }
  else if (edx == 19)
  {
    timer_free((struct TIMER *)ebx);
  }
  else if (edx == 20)
  {
    if (eax == 0) // OFF
    {
      i = io_in8(0x61);
      io_out8(0x61, i & 0x0d);
    }
    else // ON
    {
      i = 1193180000 / eax;
      io_out8(0x43, 0xb6);
      io_out8(0x42, i & 0xff);
      io_out8(0x42, i >> 8);
      i = io_in8(0x61);
      io_out8(0x61, (i | 0x03) & 0x0f);
    }
  }

  return 0;
}

void hrb_api_linewin(struct SHEET *sht, int x0, int y0, int x1, int y1,
                     int col)
{
  int i, x, y, len, dx, dy;

  dx = x1 - x0;
  dy = y1 - y0;
  x = x0 << 10;
  y = y0 << 10;
  if (dx < 0)
  {
    dx = -dx;
  }
  if (dy < 0)
  {
    dy = -dy;
  }
  if (dx >= dy)
  {
    len = dx + 1;
    if (x0 > x1)
    {
      dx = -1024;
    }
    else
    {
      dx = 1024;
    }
    if (y0 <= y1)
    {
      dy = ((y1 - y0 + 1) << 10) / len;
    }
    else
    {
      dy = ((y1 - y0 - 1) << 10) / len;
    }
  }
  else
  {
    len = dy + 1;
    if (y0 > y1)
    {
      dy = -1024;
    }
    else
    {
      dy = 1024;
    }
    if (x0 <= x1)
    {
      dx = ((x1 - x0 + 1) << 10) / len;
    }
    else
    {
      dx = ((x1 - x0 - 1) << 10) / len;
    }
  }

  for (i = 0; i < len; i++)
  {
    sht->buf[(y >> 10) * sht->bxsize + (x >> 10)] = col;
    x += dx;
    y += dy;
  }
  return;
}

int *inthandler0c(int *esp)
{
  struct TASK *task = task_now();
  struct CONSOLE *cons = task->cons;
  char s[30];
  cons_putstr0(cons, "\nINT 0C :\n General Protected Exception.\n");
  mysprintf(s, "EIP = %X\n", esp[11]);
  cons_putstr0(cons, s);
  return &(task->tss.esp0); // 異常終了させる
}

int *inthandler0d(int *esp)
{
  struct TASK *task = task_now();
  struct CONSOLE *cons = task->cons;
  char s[30];
  cons_putstr0(cons, "\nINT 0D :\n General Protected Exception.\n");
  mysprintf(s, "EIP = %X\n", esp[11]);
  cons_putstr0(cons, s);
  return &(task->tss.esp0); // 異常終了させる
}