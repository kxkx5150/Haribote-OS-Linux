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

# a_nasm
a_nasm.o: a_nasm.asm Makefile
	nasm -g -f elf -o a_nasm.o a_nasm.asm

# Cファイルをリンカスクリプトを用いてコンパイルする
bootpack.bin: $(OBJS_BOOTPACK) Makefile har.ld
	ld --allow-multiple-definition -Map=bootpack.map -m elf_i386 -e HariMain -o bootpack.bin -T har.ld $(OBJS_BOOTPACK)

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
a.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o a.o
	ld --allow-multiple-definition -Map=a.map -m elf_i386 -e HariMain -o a.hrb -T har_app.ld a_nasm.o a.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
hello3.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o hello3.o
	ld --allow-multiple-definition -Map=hello3.map -m elf_i386 -e HariMain -o hello3.hrb -T har_app.ld a_nasm.o hello3.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
hello4.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o hello4.o
	ld --allow-multiple-definition -Map=hello4.map -m elf_i386 -e HariMain -o hello4.hrb -T har_app.ld a_nasm.o hello4.o

# asmファイルをリンカスクリプトを用いてコンパイルする（アプリ）
hello5.hrb: hello5.asm Makefile
	nasm -g -f elf -o hello5.o hello5.asm
	ld --allow-multiple-definition -T har_app.ld -m elf_i386 -o $@ $(@:.hrb=.o)

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
winhelo.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o winhelo.o
	ld --allow-multiple-definition -Map=winhelo.map -m elf_i386 -e HariMain -o winhelo.hrb -T har_app.ld a_nasm.o winhelo.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
winhelo2.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o winhelo2.o
	ld --allow-multiple-definition -Map=winhelo2.map -m elf_i386 -e HariMain -o winhelo2.hrb -T har_app.ld a_nasm.o winhelo2.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
winhelo3.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o winhelo3.o
	ld --allow-multiple-definition -Map=winhelo3.map -m elf_i386 -e HariMain -o winhelo3.hrb -T har_app.ld a_nasm.o winhelo3.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
star1.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o star1.o
	ld --allow-multiple-definition -Map=star1.map -m elf_i386 -e HariMain -o star1.hrb -T har_app.ld a_nasm.o star1.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
stars.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o stars.o myrand.o
	ld --allow-multiple-definition -Map=stars.map -m elf_i386 -e HariMain -o stars.hrb -T har_app.ld a_nasm.o myrand.o stars.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
stars2.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o stars2.o myrand.o
	ld --allow-multiple-definition -Map=stars2.map -m elf_i386 -e HariMain -o stars2.hrb -T har_app.ld a_nasm.o myrand.o stars2.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
lines.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o lines.o
	ld --allow-multiple-definition -Map=lines.map -m elf_i386 -e HariMain -o lines.hrb -T har_app.ld a_nasm.o lines.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
walk.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o walk.o
	ld --allow-multiple-definition -Map=walk.map -m elf_i386 -e HariMain -o walk.hrb -T har_app.ld a_nasm.o walk.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
noodle.hrb: Makefile har_app2.ld a_nasm.o mysprintf.o noodle.o
	ld --allow-multiple-definition -Map=noodle.map -m elf_i386 -e HariMain -o noodle.hrb -T har_app2.ld a_nasm.o mysprintf.o noodle.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
beepdown.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o beepdown.o
	ld --allow-multiple-definition -Map=walk.map -m elf_i386 -e HariMain -o beepdown.hrb -T har_app.ld a_nasm.o beepdown.o

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
beepdown.hrb: a_nasm.hrb Makefile har_app.ld a_nasm.o color.o
	ld --allow-multiple-definition -Map=walk.map -m elf_i386 -e HariMain -o color.hrb -T har_app.ld a_nasm.o color.o

${OSNAME}.sys: asmhead.bin bootpack.bin Makefile
	cat asmhead.bin bootpack.bin > ${OSNAME}.sys

${OSIMAGE}: ipl10.bin ${OSNAME}.sys Makefile hello.hrb hello2.hrb hello3.hrb hello4.hrb hello5.hrb a.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb lines.hrb walk.hrb noodle.hrb beepdown.hrb color.hrb
	mformat -f 1440 -C -B ipl10.bin -i $@ ::
	mcopy -i $@ ${OSNAME}.sys ::
	mcopy -i $@ int.c ::
	mcopy -i $@ hello.hrb ::
	mcopy -i $@ hello2.hrb ::
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
	$(RM) *.hrb
	$(RM) *.sys
	$(RM) *.lst
	$(RM) *.map
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
	make docker-build
	docker run -it --rm -v $(CURDIR):/${OSNAME} -w /${OSNAME} ${OSBUILDER} make img

.PHONY: docker-run
docker-run:
	make docker-make
	qemu-system-i386 -m 32m -enable-kvm -drive file=${OSIMAGE},format=raw,if=floppy
