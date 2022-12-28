MAKE	= make -r
OSNAME = demos
OSBUILDER := ${OSNAME}-builder
OSIMAGE=${OSNAME}.img

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

ipl10.bin: ipl10.asm Makefile
	nasm ipl10.asm -o ipl10.bin -l ipl10.lst

asmhead.bin: asmhead.asm Makefile
	nasm asmhead.asm -o asmhead.bin -l ${OSNAME}.lst

nasmfunc.o: nasmfunc.asm Makefile						# nasmfunc.asmのバイナリファイル
	nasm -g -f elf nasmfunc.asm -o nasmfunc.o

bootpack.hrb: bootpack.c har.ld hankaku.c nasmfunc.o Makefile  # Cファイルをリンカスクリプトを用いてコンパイルする
	gcc -march=i486 -m32 -nostdlib -fno-pic -T har.ld -g bootpack.c hankaku.c nasmfunc.o -o bootpack.hrb

${OSNAME}.sys: asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > ${OSNAME}.sys

${OSIMAGE}: ipl10.bin ${OSNAME}.sys Makefile
	mformat -f 1440 -C -B ipl10.bin -i $@ ::
	mcopy -i $@ ${OSNAME}.sys ::

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
