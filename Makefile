# $@ = target file
# $< = first dependency
# $^ = all dependencies

C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS   = $(wildcard kernel/*.h drivers/*.h)

OBJ = ${C_SOURCES:.c=.o}

GCC  = i686-elf-gcc
LD   = i686-elf-ld
QEMU = qemu-system-x86_64

all: run

os_img.bin: bootsector.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/enter_kernel.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

bootsector.bin: boot/bootsector.asm
	nasm -f bin $< -o $@

%.o: %.c ${HEADERS}
	${GCC} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

run: os_img.bin
	${QEMU} -fda os_img.bin

clean:
	rm *.bin *.o os_img.bin *.elf
	rm kernel/*.o boot/*.bin boot/*.o drivers/*.o



