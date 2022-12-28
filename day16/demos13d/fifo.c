#include "bootpack.h"

// FIFOバッファの初期化
void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf) {
  fifo->size = size;
  fifo->buf = buf;
  fifo->free = size;  // 空き
  fifo->flags = 0;
  fifo->p = 0;  // 書き込み位置
  fifo->q = 0;  // 読み込み位置
  return;
}

// FIFOへデータを送り込んで蓄える
int fifo8_put(struct FIFO8 *fifo, unsigned char data) {
  // 空きがなくて溢れた
  if (fifo->free == 0) {
    fifo->flags |= FLAGS_OVERRUN;
    return -1;
  }
  fifo->buf[fifo->p] = data;
  fifo->p++;
  if (fifo->p == fifo->size) {
    fifo->p = 0;
  }
  fifo->free--;
  return 0;
}

// FIFOからデータを一つとってくる
int fifo8_get(struct FIFO8 *fifo) {
  int data;
  // バッファが空っぽのときは，とりあえず-1が返される
  if (fifo->free == fifo->size) {
    return -1;
  }
  data = fifo->buf[fifo->q];
  fifo->q++;
  if (fifo->q == fifo->size) {
    fifo->q = 0;
  }
  fifo->free++;
  return data;
}

// どのくらいデータがためっているかを報告する
int fifo8_status(struct FIFO8 *fifo) { return fifo->size - fifo->free; }

// FIFOバッファの初期化
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task) {
  fifo->size = size;
  fifo->buf = buf;
  fifo->free = size;  // 空き
  fifo->flags = 0;
  fifo->p = 0;        // 書き込み位置
  fifo->q = 0;        // 読み込み位置
  fifo->task = task;  // データが入ったときに起こすタスク
  return;
}

// FIFOへデータを送り込んで蓄える
int fifo32_put(struct FIFO32 *fifo, int data) {
  // 空きがなくて溢れた
  if (fifo->free == 0) {
    fifo->flags |= FLAGS_OVERRUN;
    return -1;
  }
  fifo->buf[fifo->p] = data;
  fifo->p++;
  if (fifo->p == fifo->size) {
    fifo->p = 0;
  }
  fifo->free--;
  if (fifo->task != 0) {
    if (fifo->task->flags != 2) {  // タスクが寝ていたら
      task_run(fifo->task, 0);     // 起こす
    }
  }
  return 0;
}

// FIFOからデータを一つとってくる
int fifo32_get(struct FIFO32 *fifo) {
  int data;
  // バッファが空っぽのときは，とりあえず-1が返される
  if (fifo->free == fifo->size) {
    return -1;
  }
  data = fifo->buf[fifo->q];
  fifo->q++;
  if (fifo->q == fifo->size) {
    fifo->q = 0;
  }
  fifo->free++;
  return data;
}

// どのくらいデータがためっているかを報告する
int fifo32_status(struct FIFO32 *fifo) { return fifo->size - fifo->free; }
