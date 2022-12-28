MAKE 	= make -r
OSNAME = demos
OSBUILDER := ${OSNAME}-builder
OSIMAGE=${OSNAME}.img
OBJS_BOOTPACK = int.o fifo.o graphic.o dsctbl.o \
	bootpack.o hankaku.o nasmfunc.o keyboard.o mouse.o \
	mysprintf.o memory.o sheet.o timer.o

# デフォルト動作

.PHONY: default
default :
	make img

# ファイル生成規則
## hankaku char

convHankakuTxt: convHankakuTxt.c
	$(CC) $< -o $@

hankaku.c: hankaku.txt convHankakuTxt
	./convHankakuTxt

%.bin: %.asm Makefile
	nasm $*.asm -o $*.bin -l $*.lst

# nasmfunc.asmのバイナリファイル
nasmfunc.o: nasmfunc.asm Makefile
	nasm -g -f elf -o nasmfunc.o nasmfunc.asm

# Cファイルをリンカスクリプトを用いてコンパイルする
bootpack.bin: $(OBJS_BOOTPACK) Makefile har.ld
	ld --allow-multiple-definition -m elf_i386 -e HariMain -o bootpack.bin -T har.ld $(OBJS_BOOTPACK)

${OSNAME}.sys: asmhead.bin bootpack.bin Makefile
	cat asmhead.bin bootpack.bin > ${OSNAME}.sys

${OSIMAGE}: ipl10.bin ${OSNAME}.sys Makefile
	mformat -f 1440 -C -B ipl10.bin -i $@ ::
	mcopy -i $@ ${OSNAME}.sys ::

# bootpack.hが必要であるため下ブロックと別にした
bootpack.o: bootpack.c bootpack.h
	gcc -c -m32 -fno-pic -nostdlib -o bootpack.o bootpack.c

%.o: %.c
	gcc -c -m32 -fno-pic -nostdlib -o $*.o $*.c

# コマンド
.PHONY: asm
asm:
	$(MAKE) ipl10.bin
.PHONY: img
img:
	$(MAKE) ${OSIMAGE}
.PHONY: run
run:
	$(MAKE) img
	qemu-system-i386 -drive file=${OSIMAGE},format=raw,if=floppy
.PHONY: debug
debug:
	make img
	qemu-system-i386 -drive file=${OSIMAGE},format=raw,if=floppy -gdb tcp::10000 -S
.PHONY: srcimg
srcimg:
	$(RM) *.bin
	$(RM) *.sys
	$(RM) *.lst
	$(RM) hankaku.c
	$(RM) convHankakuTxt
	$(RM) *.o
.PHONY: clean
clean:
	make srcimg
	$(RM) *.img


.PHONY: docker-build
docker-build: Dockerfile
	docker build -t ${OSBUILDER} .

.PHONY: docker-make
docker-make:
	docker run -it --rm -v $(CURDIR):/${OSNAME} -w /${OSNAME} ${OSBUILDER} make img

.PHONY: docker-run
docker-run:
	make docker-build
	make docker-make
	qemu-system-i386 -m 32m  -enable-kvm -drive file=${OSIMAGE},format=raw,if=floppy
