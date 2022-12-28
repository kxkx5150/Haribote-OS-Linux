OBJS_API=api001.o api002.o api003.o api004.o api005.o api006.o api007.o api008.o api009.o \
	api010.o api011.o api012.o api013.o api014.o api015.o api016.o api017.o api018.o api019.o api020.o

default: apilib.a

%.hrb: %.asm Makefile
	nasm $*.asm -o $*.hrb -l $*.lst

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

apilib.a: $(OBJS_API) Makefile
	ar r apilib.a $(OBJS_API)

clean :
	-$(DEL) *.lst
	-$(DEL) *.a

src_only :
	$(MAKE) clean
	-$(DEL) apilib.a