MAKE 	= make -r
OSNAME = demos
OSBUILDER := ${OSNAME}-builder
OSIMAGE=${OSNAME}.img
OBJS_BOOTPACK = int.o fifo.o graphic.o dsctbl.o \
	bootpack.o hankaku.o nasmfunc.o keyboard.o mouse.o \
	mysprintf.o memory.o sheet.o timer.o mtask.o mystrcmp.o \
	window.o console.o file.o
OBJS_API=api001.o api002.o api003.o api004.o api005.o api006.o api007.o api008.o api009.o \
	api010.o api011.o api012.o api013.o api014.o api015.o api016.o api017.o api018.o api019.o api020.o \
	api021.o api022.o api023.o api024.o api025.o api026.o \
	alloca.o

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

# API群(旧a_nasm)
api001.o: api001.asm Makefile
	nasm -g -f elf -o api001.o api001.asm
api002.o: api002.asm Makefile
	nasm -g -f elf -o api002.o api002.asm
api003.o: api003.asm Makefile
	nasm -g -f elf -o api003.o api003.asm
api004.o: api004.asm Makefile
	nasm -g -f elf -o api004.o api004.asm
api005.o: api005.asm Makefile
	nasm -g -f elf -o api005.o api005.asm
api006.o: api006.asm Makefile
	nasm -g -f elf -o api006.o api006.asm
api007.o: api007.asm Makefile
	nasm -g -f elf -o api007.o api007.asm
api008.o: api008.asm Makefile
	nasm -g -f elf -o api008.o api008.asm
api009.o: api009.asm Makefile
	nasm -g -f elf -o api009.o api009.asm
api010.o: api010.asm Makefile
	nasm -g -f elf -o api010.o api010.asm
api011.o: api011.asm Makefile
	nasm -g -f elf -o api011.o api011.asm
api012.o: api012.asm Makefile
	nasm -g -f elf -o api012.o api012.asm
api013.o: api013.asm Makefile
	nasm -g -f elf -o api013.o api013.asm
api014.o: api014.asm Makefile
	nasm -g -f elf -o api014.o api014.asm
api015.o: api015.asm Makefile
	nasm -g -f elf -o api015.o api015.asm
api016.o: api016.asm Makefile
	nasm -g -f elf -o api016.o api016.asm
api017.o: api017.asm Makefile
	nasm -g -f elf -o api017.o api017.asm
api018.o: api018.asm Makefile
	nasm -g -f elf -o api018.o api018.asm
api019.o: api019.asm Makefile
	nasm -g -f elf -o api019.o api019.asm
api020.o: api020.asm Makefile
	nasm -g -f elf -o api020.o api020.asm
api021.o: api021.asm Makefile
	nasm -g -f elf -o api021.o api021.asm
api022.o: api022.asm Makefile
	nasm -g -f elf -o api022.o api022.asm
api023.o: api023.asm Makefile
	nasm -g -f elf -o api023.o api023.asm
api024.o: api024.asm Makefile
	nasm -g -f elf -o api024.o api024.asm
api025.o: api025.asm Makefile
	nasm -g -f elf -o api025.o api025.asm
api026.o: api026.asm Makefile
	nasm -g -f elf -o api026.o api026.asm

# __alloca (harib25b)
alloca.o: alloca.asm Makefile
	nasm -g -f elf -o alloca.o alloca.asm

apilib.a: $(OBJS_API) Makefile
	ar r apilib.a $(OBJS_API)

# Cファイルをリンカスクリプトを用いてコンパイルする
bootpack.bin: $(OBJS_BOOTPACK) Makefile har.ld
	ld --allow-multiple-definition -Map=bootpack.map -m elf_i386 -e HariMain -o bootpack.bin -T har.ld $(OBJS_BOOTPACK)

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
a.hrb: Makefile har_app.ld apilib.a a.o apilib.h
	ld --allow-multiple-definition -Map=a.map -m elf_i386 -e HariMain -o a.hrb -T har_app.ld a.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
hello3.hrb: Makefile har_app.ld apilib.a hello3.o apilib.h
	ld --allow-multiple-definition -Map=hello3.map -m elf_i386 -e HariMain -o hello3.hrb -T har_app.ld hello3.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
hello4.hrb: Makefile har_app.ld apilib.a hello4.o apilib.h
	ld --allow-multiple-definition -Map=hello4.map -m elf_i386 -e HariMain -o hello4.hrb -T har_app.ld hello4.o apilib.a

# asmファイルをリンカスクリプトを用いてコンパイルする（アプリ）
hello5.hrb: hello5.asm Makefile
	nasm -g -f elf -o hello5.o hello5.asm
	ld --allow-multiple-definition -T har_app.ld -m elf_i386 -o $@ $(@:.hrb=.o)

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
winhelo.hrb: Makefile har_app.ld apilib.a winhelo.o apilib.h
	ld --allow-multiple-definition -Map=winhelo.map -m elf_i386 -e HariMain -o winhelo.hrb -T har_app.ld winhelo.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
winhelo2.hrb: Makefile har_app.ld apilib.a winhelo2.o apilib.h
	ld --allow-multiple-definition -Map=winhelo2.map -m elf_i386 -e HariMain -o winhelo2.hrb -T har_app.ld winhelo2.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
winhelo3.hrb: Makefile har_app.ld apilib.a winhelo3.o apilib.h
	ld --allow-multiple-definition -Map=winhelo3.map -m elf_i386 -e HariMain -o winhelo3.hrb -T har_app.ld winhelo3.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
star1.hrb: Makefile har_app.ld apilib.a star1.o apilib.h
	ld --allow-multiple-definition -Map=star1.map -m elf_i386 -e HariMain -o star1.hrb -T har_app.ld star1.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
stars.hrb: Makefile har_app.ld apilib.a stars.o myrand.o apilib.h
	ld --allow-multiple-definition -Map=stars.map -m elf_i386 -e HariMain -o stars.hrb -T har_app.ld myrand.o stars.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
stars2.hrb: Makefile har_app.ld apilib.a stars2.o myrand.o apilib.h
	ld --allow-multiple-definition -Map=stars2.map -m elf_i386 -e HariMain -o stars2.hrb -T har_app.ld myrand.o stars2.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
lines.hrb: Makefile har_app.ld apilib.a lines.o apilib.h
	ld --allow-multiple-definition -Map=lines.map -m elf_i386 -e HariMain -o lines.hrb -T har_app.ld lines.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
walk.hrb: Makefile har_app.ld apilib.a walk.o apilib.h
	ld --allow-multiple-definition -Map=walk.map -m elf_i386 -e HariMain -o walk.hrb -T har_app.ld walk.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
noodle.hrb: Makefile har_app2.ld apilib.a mysprintf.o noodle.o apilib.h
	ld --allow-multiple-definition -Map=noodle.map -m elf_i386 -e HariMain -o noodle.hrb -T har_app2.ld mysprintf.o noodle.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
beepdown.hrb: Makefile har_app.ld apilib.a beepdown.o apilib.h
	ld --allow-multiple-definition -Map=walk.map -m elf_i386 -e HariMain -o beepdown.hrb -T har_app.ld beepdown.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
color.hrb: Makefile har_app.ld apilib.a color.o apilib.h
	ld --allow-multiple-definition -Map=walk.map -m elf_i386 -e HariMain -o color.hrb -T har_app.ld color.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
color2.hrb: Makefile har_app.ld apilib.a color2.o apilib.h
	ld --allow-multiple-definition -Map=walk.map -m elf_i386 -e HariMain -o color2.hrb -T har_app.ld color2.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
sosu.hrb: Makefile har_app.ld apilib.a mylib.h mysprintf.o sosu.o apilib.h
	ld --allow-multiple-definition -Map=walk.map -m elf_i386 -e HariMain -o sosu.hrb -T har_app.ld mysprintf.o sosu.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
sosu2.hrb: Makefile har_app.ld apilib.a mylib.h mysprintf.o sosu2.o apilib.h
	ld --allow-multiple-definition -Map=walk.map -m elf_i386 -e HariMain -o sosu2.hrb -T har_app.ld mysprintf.o sosu2.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
sosu3.hrb: Makefile har_app.ld apilib.a mylib.h mysprintf.o sosu3.o apilib.h
	ld --allow-multiple-definition -Map=walk.map -m elf_i386 -e HariMain -o sosu3.hrb -T har_app.ld mysprintf.o sosu3.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
typeipl.hrb: Makefile har_app.ld apilib.a mylib.h typeipl.o apilib.h
	ld --allow-multiple-definition -Map=typeipl.map -m elf_i386 -e HariMain -o typeipl.hrb -T har_app.ld typeipl.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
type.hrb: Makefile har_app.ld apilib.a mylib.h type.o apilib.h
	ld --allow-multiple-definition -Map=type.map -m elf_i386 -e HariMain -o type.hrb -T har_app.ld type.o apilib.a

# Cファイルをリンカスクリプトを用いてコンパイルする（アプリ）
iroha.hrb: Makefile har_app.ld apilib.a mylib.h iroha.o apilib.h
	ld --allow-multiple-definition -Map=iroha.map -m elf_i386 -e HariMain -o iroha.hrb -T har_app.ld iroha.o apilib.a

${OSNAME}.sys: asmhead.bin bootpack.bin Makefile
	cat asmhead.bin bootpack.bin > ${OSNAME}.sys

${OSIMAGE}: ipl10.bin ${OSNAME}.sys Makefile hello3.hrb hello4.hrb hello5.hrb a.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb lines.hrb walk.hrb noodle.hrb beepdown.hrb color.hrb color2.hrb sosu.hrb sosu2.hrb sosu3.hrb ipl10.asm typeipl.hrb type.hrb iroha.hrb
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
	mcopy -i $@ sosu.hrb ::
	mcopy -i $@ sosu2.hrb ::
	mcopy -i $@ sosu3.hrb ::
	mcopy -i $@ ipl10.asm ::
	mcopy -i $@ typeipl.hrb ::
	mcopy -i $@ type.hrb ::
	mcopy -i $@ iroha.hrb ::
	mcopy -i $@ nihongo.fnt ::


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

.PHONY: docker-run
docker-run:
	make docker-make
	qemu-system-i386 -m 32m -enable-kvm -drive file=${OSIMAGE},format=raw,if=floppy
