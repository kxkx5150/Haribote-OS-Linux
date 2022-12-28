#include "bootpack.h"

void init_gdtidt(void) {
  struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)ADR_GDT;
  struct GATE_DESCRIPTOR *idt = (struct GATE_DESCRIPTOR *)ADR_IDT;
  int i;

  /*GDTの初期化*/
  // 8192個のGDTすべてのセグメントに対して，リミット0，ベース0，アクセス権属性0を付与
  for (i = 0; i <= LIMIT_GDT / 8; i++) {
    set_segmdesc(gdt + i, 0, 0, 0);  //ポインタへの足し算は，実際は掛け算
  }
  set_segmdesc(gdt + 1, 0xffffffff, 0x00000000,
               AR_DATA32_RW);  // セグメント番号1番に対する設定
  set_segmdesc(
      gdt + 2, 0x0007ffff, 0x00280000,
      AR_CODE32_ER);  // セグメント番号2番に対する設定（bootpack.hrb用）
  load_gdtr(LIMIT_GDT, ADR_GDT);

  /* IDTの初期化 */
  for (i = 0; i <= LIMIT_IDT / 8; i++) {
    set_gatedesc(idt + i, 0, 0, 0);
  }
  load_idtr(LIMIT_IDT, ADR_IDT);

  // IDTの設定
  set_gatedesc(idt + 0x0d, (int)asm_inthandler0d, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x20, (int)asm_inthandler20, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x21, (int)asm_inthandler21, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x27, (int)asm_inthandler27, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x2c, (int)asm_inthandler2c, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x40, (int)asm_hrb_api, 2 * 8, AR_INTGATE32 + 0x60);
  return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base,
                  int ar) {
  if (limit > 0xfffff) {
    ar |= 0x8000; /* G_bit = 1*/
    limit /= 0x1000;
  }
  sd->limit_low = limit & 0xffff;
  sd->base_low = base & 0xffff;
  sd->base_mid = (base >> 16) & 0xff;
  sd->access_right = ar & 0xff;
  sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
  sd->base_high = (base >> 24) & 0xff;
  return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector,
                  int ar) {
  gd->offset_low = offset & 0xffff;
  gd->selector = selector;
  gd->dw_count = (ar >> 8) & 0xff;
  gd->access_right = ar & 0xff;
  gd->offset_high = (offset >> 16) & 0xffff;
  return;
}
