#include "memory.h"
#include "kheap.h"
#include "libc.h"

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

#define MAKE_FLAGS(is_present, is_writable, is_kernel) ((is_present & 0x1) | ((is_writable & 0x1) << 1) | (((~is_kernel) & 0x1) << 2))
// kernel page directory
page_directory* kernel_directory;

// current directory
page_directory* current_directory;

// basic bitmap variables
unsigned int* frames;
unsigned int nframes;

// from "kheap.c"
extern unsigned int placement_address;

// load a page directory to cr3
void load_cr3(page_directory* pd){
	current_directory = pd;
	asm volatile("mov %0, %%cr3":: "r"(&(pd->tables_physical)));
	unsigned int cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}


// set bit in frame bitmap
void set_frame(unsigned int frame_addr){
	unsigned int frame = frame_addr/0x1000;
	unsigned int index = INDEX_FROM_BIT(frame);
	unsigned int off   = OFFSET_FROM_BIT(frame);
	frames[index] |= (0x1 << off);
}

// clear bit in frame bitmap
void clear_frame(unsigned int frame_addr){
	unsigned int frame = frame_addr/0x1000;
	unsigned int index = INDEX_FROM_BIT(frame);
	unsigned int off = OFFSET_FROM_BIT(frame);
	frames[index] &= ~(0x1 << off);
}

// test if frame in bitmap is set
unsigned int test_frame(unsigned int frame_addr){
	unsigned int frame = frame_addr/0x1000;
	unsigned int index = INDEX_FROM_BIT(frame);
	unsigned int off = OFFSET_FROM_BIT(frame);
	return (frames[index] & (0x1 << off));
}

// find the first free frame
unsigned int first_frame(){
	for(unsigned int i = 0; i < INDEX_FROM_BIT(nframes); i++ )
	{	
		if(frames[i] != 0xFFFFFFFF)
		{
			for(unsigned int j = 0; j < 32; j++)
			{
				unsigned int to_test = 0x1 << j;
				if(!(frames[i] & to_test))
				{
					return i*4*8 + j;
				}
			}
		}
	}
	return -1;
}

// function to free a frame
void free_frame(page_table_entry *page){
	unsigned int frame = page->page_base;

	if(!frame)
		return;

	clear_frame(frame);
	page->page_base = 0;
}

// allocate a frame
void alloc_frame(page_table_entry* page, int kernelmode, int writeable){
	if(page->page_base != 0)
		return;

	unsigned int index = first_frame();

	if(index == (unsigned int) -1){
		kprint("No free frames!\n");
		while(1);
	}

	set_frame(index * 0x1000);

	page->flags = MAKE_FLAGS(1, writeable, kernelmode);
	page->page_base = index;
}

page_table_entry* get_page(unsigned int address, int make, page_directory* dir){
	address /= 0x1000;

	unsigned int table_index = address / 1024;

	if(dir->tables[table_index])
		return &(dir->tables[table_index]->pages[address % 1024]);
	
	else if(make){
		unsigned int tmp  = (unsigned int) kmalloc(sizeof(page_table), 1);

		dir->tables[table_index] = (page_table*) tmp;
		dir->tables_physical[table_index] = tmp | MAKE_FLAGS(1, 1, 0);

		return &dir->tables[table_index]->pages[address % 1024];
	}

	return 0;
}

void page_fault_handler(context_t ctx){
	kprint("PAGE FAULT!\n");
	while(1);
}

void init_paging(){
	unsigned int mem_end_page = 0x1000000;

	nframes = mem_end_page / 0x1000;
	frames = (unsigned int*)kmalloc(INDEX_FROM_BIT(nframes), 0);

	memset(frames, 0, INDEX_FROM_BIT(nframes));

	kernel_directory = (page_directory*) kmalloc(sizeof(page_directory) , 1);
	current_directory = kernel_directory;

	int i = 0;
	while(i < placement_address){
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}

	register_interrupt_handler(14, page_fault_handler);

	load_cr3(kernel_directory);
}
