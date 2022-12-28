#include "apilib.h"

unsigned long myrand(void);

void HariMain(void)
{
  char *buf;
  int win, i, x, y;
  api_initmalloc();
  buf = api_malloc(150 * 100);
  win = api_openwin(buf, 150, 100, -1, "stars");
  api_boxfilwin(win, 6, 26, 143, 93, 0); // 0: 黒色
  for (i = 0; i < 50; i++)
  {
    x = (myrand() % 137) + 6;
    y = (myrand() % 67) + 26;
    api_point(win, x, y, 3); // 3: 黄色
  }

  api_end();
}