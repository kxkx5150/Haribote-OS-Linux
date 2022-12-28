MAKE	= make -r
image_file=demos.img
# デフォルト動作

default :
	make img

# ファイル生成規則

ipl10.bin: ipl10.asm Makefile
	nasm ipl10.asm -o ipl10.bin -l ipl10.lst

asmhead.bin: asmhead.asm Makefile
	nasm asmhead.asm -o asmhead.bin -l demos.lst

bootpack.hrb: bootpack.c har.ld Makefile  # Cファイルをリンカスクリプトを用いてコンパイルする
	gcc -march=i486 -m32 -nostdlib -fno-pic -T har.ld bootpack.c -o bootpack.hrb

demos.sys: asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > demos.sys

image_file: ipl10.bin demos.sys Makefile
	mformat -f 1440 -C -B ipl10.bin -i ${image_file} ::
	mcopy demos.sys -i ${image_file} ::

# コマンド

asm:
	$(MAKE) ipl10.bin
img:
	$(MAKE) image_file
run:
	$(MAKE) img
	qemu-system-i386 -drive file=${image_file},format=raw,if=floppy
debug:
	make img
	qemu-system-i386 -drive file=${image_file},format=raw,if=floppy -gdb tcp::10000 -S
clean:
	$(RM) *.bin
	$(RM) *.sys
	$(RM) *.lst
	$(RM) bootpack.hrb
src_only:
	make clean
	$(RM) *.img