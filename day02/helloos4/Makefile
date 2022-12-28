default:
	make img

# ファイル生成規則

ipl.bin: ipl.asm Makefile
	nasm ipl.asm -o ipl.bin -l ipl.lst
tail.bin: tail.asm Makefile
	nasm tail.asm -o tail.bin -l tail.lst
helloos.img: ipl.bin tail.bin Makefile
	cat ipl.bin tail.bin > helloos.img

asm:
	make -r ipl.bin
img:
	make -r helloos.img
run:
	make img
	qemu-system-i386 helloos.img
