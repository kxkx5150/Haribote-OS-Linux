MAKE	= make -r

# デフォルト動作

default :
	make img

# ファイル生成規則

ipl.bin: ipl.asm Makefile
	nasm ipl.asm -o ipl.bin -l ipl.lst
tail.bin: tail.asm Makefile
	nasm tail.asm -o tail.bin -l tail.lst
demos.img: ipl.bin tail.bin Makefile
	cat ipl.bin tail.bin > demos.img

# コマンド

asm:
	$(MAKE) ipl.bin
img:
	$(MAKE) demos.img
run:
	$(MAKE) img
	qemu-system-i386 -drive file=demos.img,format=raw,if=floppy
clean:
	$(RM) *.bin
	$(RM) *.lst
src_only:
	make clean
	$(RM) *.img