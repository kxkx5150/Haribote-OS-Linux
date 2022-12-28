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

%.bin: %.asm Makefile
	nasm $*.asm -o $*.bin -l $*.lst

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
crack1.hrb: a_nasm.hrb Makefile har_app.ld crack1.o
	ld --allow-multiple-definition -Map=crack1.map -m elf_i386 -e HariMain -o crack1.hrb -T har_app.ld crack1.o

${OSNAME}.sys: asmhead.bin bootpack.bin Makefile
	cat asmhead.bin bootpack.bin > ${OSNAME}.sys

${OSIMAGE}: ipl10.bin ${OSNAME}.sys Makefile hello.hrb hello2.hrb hello3.hrb a.hrb crack1.hrb
	mformat -f 1440 -C -B ipl10.bin -i $@ ::
	mcopy -i $@ ${OSNAME}.sys ::
	mcopy -i $@ int.c ::
	mcopy -i $@ hello.hrb ::
	mcopy -i $@ hello2.hrb ::
	mcopy -i $@ hello3.hrb ::
	mcopy -i $@ a.hrb ::
	mcopy -i $@ crack1.hrb ::


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
	docker run -it --rm -v $(CURDIR):/${OSNAME} -w /${OSNAME} ${OSBUILDER} make img

.PHONY: docker-run
docker-run:
	make docker-build
	make docker-make
	qemu-system-i386 -m 32m -enable-kvm -drive file=${OSIMAGE},format=raw,if=floppy
