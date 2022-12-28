MAKE	= make -r
image_file=demos.img
# デフォルト動作

default :
	make img

# ファイル生成規則

ipl.bin: ipl.asm Makefile
	nasm ipl.asm -o ipl.bin -l ipl.lst

demos.sys: demos.asm Makefile
	nasm demos.asm -o demos.sys -l demos.lst

image_file: ipl.bin demos.sys Makefile
	mformat -f 1440 -C -B ipl.bin -i ${image_file} ::
	mcopy demos.sys -i ${image_file} ::

# コマンド

asm:
	$(MAKE) ipl.bin
img:
	$(MAKE) image_file
run:
	$(MAKE) img
	qemu-system-i386 -drive file=${image_file},format=raw,if=floppy
clean:
	$(RM) *.bin
	$(RM) *.sys
	$(RM) *.lst
src_only:
	make clean
	$(RM) *.img