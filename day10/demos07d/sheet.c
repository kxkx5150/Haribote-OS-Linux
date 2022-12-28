#include "bootpack.h"

struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize)
{
    struct SHTCTL *ctl;
    int i;
    ctl = (struct SHTCTL *)memman_alloc_4k(memman, sizeof(struct SHTCTL));
    if (ctl == 0)
    {
        goto err;
    }
    ctl->vram = vram;
    ctl->xsize = xsize;
    ctl->ysize = ysize;
    ctl->top = -1; // 一枚もない
    for (i = 0; i < MAX_SHEETS; i++)
    {
        ctl->sheets0[i].flags = 0; // 未使用マーク
    }
err:
    return ctl;
}

struct SHEET *sheet_alloc(struct SHTCTL *ctl)
{
    struct SHEET *sht;
    int i;
    for (i = 0; i < MAX_SHEETS; i++)
    {
        if (ctl->sheets0[i].flags == 0)
        {
            sht = &ctl->sheets0[i];
            sht->flags = SHEET_USE; // 使用中マーク
            sht->height = -1;       // 非表示
            return sht;
        }
    }
    return 0; //すべてのシートが使用中だった
}

// 下敷きのバッファや大きさ、透明色を設定する関数
void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv)
{
    sht->buf = buf;
    sht->bxsize = xsize;
    sht->bysize = ysize;
    sht->col_inv = col_inv;
    return;
}

// 下敷きの高さを設定するための関数
// *ctl: 下敷きコントロール構造体
// *sht: 対象シート
// height: 設定する高さ
void sheet_updown(struct SHTCTL *ctl, struct SHEET *sht, int height)
{
    int h;
    int old = sht->height; // 設定前の高さを記憶

    // 設定が低すぎや高すぎだったら、修正する
    if (height > ctl->top + 1)
    {
        height = ctl->top + 1;
    }
    if (height < -1)
    {
        height = -1;
    }
    sht->height = height; //高さを設定

    // sheets[]の並び替え
    if (old > height)
    { // 以前よりも低くなる
        if (height >= 0)
        {
            // 間のものを引き上げる
            for (h = old; h > height; h--)
            {
                ctl->sheets[h] = ctl->sheets[h - 1];
                ctl->sheets[h]->height = h;
            }
            ctl->sheets[height] = sht;
        }
        else
        { // 非表示化（-1）
            if (ctl->top > old)
            {
                // 上になっているものを下ろす
                for (h = old; h < ctl->top; h++)
                {
                    ctl->sheets[h] = ctl->sheets[h + 1];
                    ctl->sheets[h]->height = h;
                }
            }
            ctl->top--; // 表示中の下敷きが一つ減るので、一番上の高さが減る
        }
        sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize); // 新しい下敷きの情報に沿って画面を書き直す
    }
    else if (old < height)
    { //以前よりも高くなる
        if (old >= 0)
        {
            // 間のものを押し下げる
            for (h = old; h < height; h++)
            {
                ctl->sheets[h] = ctl->sheets[h + 1];
                ctl->sheets[h]->height = h;
            }
            ctl->sheets[height] = sht;
        }
        else
        { // 非表示状態から表示状態へ
            // 上になるものを持ち上げる
            for (h = ctl->top; h >= height; h--)
            {
                ctl->sheets[h + 1] = ctl->sheets[h];
                ctl->sheets[h + 1]->height = h + 1;
            }
            ctl->sheets[height] = sht;
            ctl->top++; // 表示中の下敷きが一つ増えるので、一番上の高さが増える
        }
        sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize); // 新しい下敷きの情報に沿って画面を書き直す
    }
    return;
}

// 下敷きを下から全部描いていく関数
void sheet_refresh(struct SHTCTL *ctl, struct SHEET *sht, int bx0, int by0, int bx1, int by1)
{
    if (sht->height >= 0)
    {
        sheet_refreshsub(ctl, sht->vx0 + bx0, sht->vy0, sht->vx0 + bx1, sht->vy0 + by1);
    }
    return;
}

// 範囲指定をして一部分だけ書き換える
void sheet_refreshsub(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1)
{
    int h, bx, by, vx, vy, bx0, by0, bx1, by1;
    unsigned char *buf, c, *vram = ctl->vram;
    struct SHEET *sht;
    for (h = 0; h <= ctl->top; h++)
    {
        sht = ctl->sheets[h];
        buf = sht->buf;
        // vx0~vy1を使って、bx0~by1を逆算する
        bx0 = vx0 - sht->vx0;
        by0 = vy0 - sht->vy0;
        bx1 = vx1 - sht->vx0;
        by1 = vy1 - sht->vy0;
        if (bx0 < 0)
        {
            bx0 = 0;
        }
        if (by0 < 0)
        {
            by0 = 0;
        }
        if (bx1 > sht->bxsize)
        {
            bx1 = sht->bxsize;
        }
        if (by1 > sht->bysize)
        {
            by1 = sht->bysize;
        }
        for (by = by0; by < by1; by++)
        {
            vy = sht->vy0 + by;
            for (bx = bx0; bx < bx1; bx++)
            {
                vx = sht->vx0 + bx;
                c = buf[by * sht->bxsize + bx];
                if (c != sht->col_inv)
                {
                    vram[vy * ctl->xsize + vx] = c;
                }
            }
        }
    }
    return;
}

// 下敷きを上下に動かす関数
void sheet_slide(struct SHTCTL *ctl, struct SHEET *sht, int vx0, int vy0)
{
    int old_vx0 = sht->vx0, old_vy0 = sht->vy0;
    sht->vx0 = vx0;
    sht->vy0 = vy0;
    if (sht->height >= 0)
    { // もし表示中なら新しい下敷きの情報に沿って画面を描き直す
        sheet_refreshsub(ctl, old_vx0, old_vy0, old_vx0 + sht->bxsize, old_vy0 + sht->bysize);
        sheet_refreshsub(ctl, vx0, vy0, vx0 + sht->bxsize, vy0 + sht->bysize);
    }
    return;
}

// 下敷きを開放する関数
void sheet_free(struct SHTCTL *ctl, struct SHEET *sht)
{
    if (sht->height >= 0)
    {
        sheet_updown(ctl, sht, -1); // 表示中ならまず非表示にする
    }
    sht->flags = 0; // 未使用マーク
    return;
}