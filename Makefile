C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS   = $(wildcard kernel/*.h drivers/*.h)

OBJ = ${C_SOURCES:.c=.o kernel/idt_common.o}

#OBJ = ${C_SOURCES:.c=.o}

CC  = i686-elf-gcc
LD  = i686-elf-ld
QMU = qemu-system-x86_64

os_img.bin: boot/bootsector.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/enter_kernel.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/enter_kernel.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ 

run: os_img.bin
	${QMU} -fda $<

%.o: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm  *.bin  *.o os-image.bin *.elf
	rm  kernel/*.o kernel/*.bin boot/*.bin drivers/*.o boot/*.o

