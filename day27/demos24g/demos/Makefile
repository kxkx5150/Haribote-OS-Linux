MAKE 	= make -r
OSNAME = demos
OSBUILDER := ${OSNAME}-builder
OSIMAGE=${OSNAME}.img
OBJS_BOOTPACK = int.o fifo.o graphic.o dsctbl.o \
	bootpack.o hankaku.o nasmfunc.o keyboard.o mouse.o \
	mysprintf.o memory.o sheet.o timer.o mtask.o mystrcmp.o \
	window.o console.o file.o

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

ipl10.bin: ipl10.asm
	nasm $^ -o $@ -l $(@:.bin=.list)

asmhead.bin: asmhead.asm
	nasm $^ -o $@

%.hrb: %.asm Makefile
	nasm $*.asm -o $*.hrb -l $*.lst

# nasmfunc.asmのバイナリファイル
nasmfunc.o: nasmfunc.asm Makefile
	nasm -g -f elf -o nasmfunc.o nasmfunc.asm

# Cファイルをリンカスクリプトを用いてコンパイルする
bootpack.bin: $(OBJS_BOOTPACK) Makefile har.ld
	ld --allow-multiple-definition -Map=bootpack.map -m elf_i386 -e HariMain -o bootpack.bin -T har.ld $(OBJS_BOOTPACK)

${OSNAME}.sys: asmhead.bin bootpack.bin Makefile
	cat asmhead.bin bootpack.bin > ${OSNAME}.sys

${OSIMAGE}: ipl10.bin ${OSNAME}.sys Makefile hello3.hrb hello4.hrb hello5.hrb a.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb lines.hrb walk.hrb noodle.hrb beepdown.hrb color.hrb color2.hrb
	mformat -f 1440 -C -B ipl10.bin -i $@ ::
	mcopy -i $@ ${OSNAME}.sys ::
	mcopy -i $@ hello3.hrb ::
	mcopy -i $@ hello4.hrb ::
	mcopy -i $@ hello5.hrb ::
	mcopy -i $@ a.hrb ::
	mcopy -i $@ winhelo.hrb ::
	mcopy -i $@ winhelo2.hrb ::
	mcopy -i $@ winhelo3.hrb ::
	mcopy -i $@ star1.hrb ::
	mcopy -i $@ stars.hrb ::
	mcopy -i $@ stars2.hrb ::
	mcopy -i $@ lines.hrb ::
	mcopy -i $@ walk.hrb ::
	mcopy -i $@ noodle.hrb ::
	mcopy -i $@ beepdown.hrb ::
	mcopy -i $@ color.hrb ::
	mcopy -i $@ color2.hrb ::

# bootpack.hが必要であるため下ブロックと別にした
bootpack.o: bootpack.c bootpack.h
	gcc -c -m32 -fno-pic -nostdlib -o bootpack.o bootpack.c

# TODO: C以外もoを求めることがあるので、この記述はなんとかしたい
%.o: %.c
	gcc -c -m32 -fno-pic -nostdlib -o $*.o $*.c

# コマンド
.PHONY: asm
asm:
	$(MAKE) ipl10.bin

.PHONY: img
img:
	$(MAKE) ${OSIMAGE}

.PHONY: debug
debug:
	make img
	qemu-system-i386 -drive file=${OSIMAGE},format=raw,if=floppy -gdb tcp::10000 -S

.PHONY: srcimg
srcimg:
	$(RM) *.bin
	$(RM) *.hrb
	$(RM) *.sys
	$(RM) *.lst
	$(RM) *.map
	$(RM) hankaku.c
	$(RM) convHankakuTxt
	$(RM) *.o
	$(RM) *.a

.PHONY: clean
clean:
	make srcimg
	$(RM) *.img


.PHONY: docker-build
docker-build: Dockerfile
	docker build -t ${OSBUILDER} .

.PHONY: docker-make
docker-make:
	make docker-build
	docker run -it --rm -v $(CURDIR):/${OSNAME} -w /${OSNAME} ${OSBUILDER} make img
