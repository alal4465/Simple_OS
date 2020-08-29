#include "kheap.h"

// end is defined in the linker script.
extern unsigned int end;
unsigned int placement_address = (unsigned int) &end;

malloc_bin fast_bin =  {0};
malloc_bin small_bin = {0};
malloc_bin large_bin = {0};

void* kmalloc(unsigned int sz, int page_align) {
	if (!sz)
		return 0;

	sz = ROUND_UP(sz, 8);

	malloc_bin*   bin;
	malloc_bin*   current_bin;
	malloc_chunk* chunk;

	// choose the correct bin with respected to the required size
	if (sz <= FASTBIN_MAX_SIZE)
		bin = &fast_bin;
	else if (sz <= SMALLBIN_MAX_SIZE)
		bin = &small_bin;
	else
		bin = &large_bin;

	current_bin = bin;

	// find a free chunk in the current bin
	// that is within the bounds of the required size
	chunk = current_bin->flink;
	malloc_chunk* FD;
	malloc_chunk* BK;
	while (chunk) {
		current_bin = (malloc_bin*) &chunk->links;

		if (chunk->mchunk_size >= sz && chunk->page_aligned == page_align) {
			UNLINK(current_bin, BK, FD);

			return set_chunk(
				chunk,
				chunk->mchunk_size,
				page_align,
				0,
				0);
		}

		chunk = current_bin->flink;
	}

	// align placement_address if needed
	if (page_align == 1 && (placement_address & 0xFFFFF000))
	{
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}

	// if no free fitting chunk, create one
	malloc_chunk* ret_chunk = (malloc_chunk*) placement_address;
	placement_address += (sz + sizeof(malloc_chunk));

	return set_chunk(
		ret_chunk,
		sz,
		page_align,
		0,
		0
	);
}

void* set_chunk(
	malloc_chunk* chunk,
	unsigned int sz,
	unsigned int page_aligned,
	malloc_chunk* prev,
	malloc_chunk* next)
{
	// initialize a chunk and return a pointer to it's data
	chunk->page_aligned = page_aligned;
	chunk->mchunk_size = sz;
	chunk->links.blink = prev;
	chunk->links.flink = next;

	return (void*) (((unsigned char*) &chunk->links) + sizeof(malloc_bin));
}

void  kfree(void* ptr) {
	if (!ptr)
		return;

	malloc_chunk* chunk =(malloc_chunk*) ((unsigned char*)ptr - sizeof(malloc_chunk));
	unsigned int sz = chunk->mchunk_size;

	if (!sz)
		return;

	malloc_bin* bin;

	// choose the correct bin with respected to the required size
	if (sz <= FASTBIN_MAX_SIZE)
		bin = &fast_bin;
	else if (sz <= SMALLBIN_MAX_SIZE)
		bin = &small_bin;
	else
		bin = &large_bin;

	//add it to the corresponding bin
	malloc_chunk* fd = bin->flink;

	if (fd) {
		bin->flink = chunk;
		fd->links.blink = chunk;

		chunk->links.flink = fd;
		chunk->links.blink = 0;
	}
	else
		bin->flink = chunk;
}
