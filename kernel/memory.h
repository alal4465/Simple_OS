#ifndef MEMORY_H
#define MEMORY_H
#include "isr.h"

/*
typedef struct s_directory_entry{
	unsigned int flags : 9;
	unsigned short reserved : 3;
	unsigned int pt_base : 20;	
} __attribute__((packed)) page_directory_entry;
*/

typedef struct s_table_entry{
        unsigned int flags : 9;
        unsigned short reserved : 3;
        unsigned int page_base : 20;
} __attribute__((packed)) page_table_entry;

typedef struct s_page_table
{
	page_table_entry pages[1024];
} __attribute((packed)) page_table;

typedef struct s_page_directory{
	// array of pointers to pagetables
	page_table* tables[1024];

	// array of pointers to the above pagetables
	// with their physical location for loading into CR3
	unsigned int tables_physical[1024];

	// The physical address of tables_physical
	unsigned int physical_addr;
} __attribute((packed)) page_directory;

void init_paging();
void page_fault_handler(context_t ctx);
page_table_entry* get_page(unsigned int address, int make, page_directory* dir);

#endif /* MEMORY_H  */
