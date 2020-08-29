#ifndef H_KHEAP
#define H_KHEAP
struct heap_chunk_t;

typedef struct bin_t {
	struct heap_chunk_t* flink;  // forward link
	struct heap_chunk_t* blink;  // backword link
} malloc_bin;

typedef struct heap_chunk_t {
	unsigned int         page_aligned: 1;   // set if chunk data is page aligned
	unsigned int         mchunk_size : 31;  // size in bytes, including overhead
	malloc_bin           links;             // forward and backword links
} malloc_chunk;

#define FASTBIN_MAX_SIZE  64
#define SMALLBIN_MAX_SIZE 1024

#define ROUND_UP(n, mod) (!(n%mod) ? n : (mod - (n % mod)) + n)

#define UNLINK(lnk, BK, FD){\
FD = lnk->flink;\
BK = lnk->blink;\
if(FD)\
	FD->links.blink = BK;\
if(BK)\
	BK->links.flink = FD;\
}

void  kfree(void* ptr);
void* kmalloc(unsigned int sz, int page_align);
void* set_chunk(
	malloc_chunk* chunk,
	unsigned int sz,
	unsigned int page_aligned,
	malloc_chunk* prev,
	malloc_chunk* next);

#endif /* H_KHEAP  */
