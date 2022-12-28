MAKE	= make -r
OSNAME = demos
OSBUILDER := ${OSNAME}-builder
OSIMAGE=${OSNAME}.img
CFLAGS=-fno-pie -march=i486 -m32 -nostdlib -fno-builtin
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

hankaku.o: hankaku.c
	gcc -c -m32 -fno-pic -nostdlib -o hankaku.o hankaku.c

mysprintf.o: mysprintf.c
	gcc -c -m32 -fno-pic -nostdlib -o mysprintf.o mysprintf.c


%.bin: %.asm Makefile
	nasm $*.asm -o $*.bin -l $*.lst

nasmfunc.o: nasmfunc.asm Makefile				# nasmfunc.asmのバイナリファイル
	nasm -g -f elf -o nasmfunc.o nasmfunc.asm

bootpack.bin: bootpack.o hankaku.o nasmfunc.o mysprintf.o dsctbl.o graphic.o
	ld --allow-multiple-definition -m elf_i386 -e HariMain -o bootpack.bin -T har.ld bootpack.o graphic.o dsctbl.o hankaku.o nasmfunc.o mysprintf.o

${OSNAME}.sys: asmhead.bin bootpack.bin Makefile
	cat asmhead.bin bootpack.bin > ${OSNAME}.sys

${OSIMAGE}: ipl10.bin ${OSNAME}.sys Makefile
	mformat -f 1440 -C -B ipl10.bin -i $@ ::
	mcopy -i $@ ${OSNAME}.sys ::

graphic.o: graphic.c
	gcc -c -m32 -fno-pic -nostdlib -o graphic.o graphic.c

dsctbl.o: dsctbl.c
	gcc -c -m32 -fno-pic -nostdlib -o dsctbl.o dsctbl.c

bootpack.o: bootpack.c
	gcc -c -m32 -fno-pic -nostdlib -o bootpack.o bootpack.c



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
.PHONY: clean
clean:
	$(RM) *.bin
	$(RM) *.sys
	$(RM) *.lst
	$(RM) bootpack.hrb
	$(RM) hankaku.c
	$(RM) convHankakuTxt
	$(RM) *.o
.PHONY: src_only
src_only:
	make clean
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
	qemu-system-i386 -drive file=${OSIMAGE},format=raw,if=floppy
