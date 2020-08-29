C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS   = $(wildcard kernel/*.h drivers/*.h)
OBJ       = ${C_SOURCES:.c=.o kernel/idt_common.o}

CC  = i686-elf-gcc
LD  = i686-elf-ld
QMU = qemu-system-x86_64

lnk_script = link.ld

os_img.bin: boot/bootsector.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/enter_kernel.o ${OBJ}
	${LD} -o $@ -T${lnk_script} $^ --oformat binary

kernel.elf: boot/enter_kernel.o ${OBJ}
	${LD} -o $@ -T${lnk_script} $^ 

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
