#include "../drivers/screen.h"
#include "idt.h"
#include "memory.h"
#include "kheap.h"
#include "libc.h"

void main(){

	screen_clear();

	char *kernel_message = "Started the kernel successfully!\n";
	kprint(kernel_message);

	init_idt();

	__asm__("int $0x00");
	__asm__("int $0x80");

	init_paging();
}
