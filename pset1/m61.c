#define M61_DISABLE 1
#include "m61.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

// *** global variables ***
struct m61_statistics statistics = {0, 0, 0, 0, 0 , 0, 0, 0}; // *** initialize all members of the struct to zero. It is like initializing an array with 8 zeros. ***



/// m61_malloc(sz, file, line)
///    Return a pointer to `sz` bytes of newly-allocated dynamic memory.
///    The memory is not initialized. If `sz == 0`, then m61_malloc may
///    either return NULL or a unique, newly-allocated pointer value.
///    The allocation request was at location `file`:`line`.

void* m61_malloc(size_t sz, const char* file, int line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.

    void* ptr;  // *** declares variable ptr that will hold the sz (in bytes) + metadata (in bytes) ***
    struct m61_metadata metadata;   // *** declares the variable of type struct m61_metadata ***

    if (sz ==0) {   // *** ***
        return NULL;    // *** ***
    }

    statistics.nactive++;   // *** increase nactive in the statistics struct by one. ***
    statistics.ntotal++;   // *** increase ntotal in the statistics struct by one. Every time an allocation is added it increases. ***
    statistics.total_size += sz;    // *** increase total_size by sz ***

    metadata.size = sz; // *** this is the size that it is allocated ***

    ptr = base_malloc(sz + sizeof(struct m61_metadata)); // *** ptr stores the pointer to the allocated memory which is sz (in bytes) plus metadata (in bytes) ***

    memcpy (ptr, &metadata, sizeof(struct m61_metadata));  // *** copies the metadata to the allocated memory ***

    return ptr + sizeof(struct m61_metadata);   // *** returns the sz bytes without metadata (the metadata is for me not the user) ***
}


/// m61_free(ptr, file, line)
///    Free the memory space pointed to by `ptr`, which must have been
///    returned by a previous call to m61_malloc and friends. If
///    `ptr == NULL`, does nothing. The free was called at location
///    `file`:`line`.

void m61_free(void *ptr, const char *file, int line) {

    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.

    if (ptr) {
    struct m61_metadata* metadata; // *** metadata is now a pointer to the m61_metadata structure ***
    size_t sz;  // *** declares variable that is going to keep the size later (the size of user allocated memory) ***


    statistics.nactive--;   // *** decrease nactive in the statistics structs by one (b/c I am freeing it) ***

    metadata = (struct m61_metadata*)(ptr - sizeof(metadata));  // *** casting to struct m61_metadata* (pointer is equal to pointer of same type) ***

    sz = metadata->size;    // *** I retreive the metadata and that is my size and the I retrieve the size of allocated memory from metadata ***

    base_free(ptr-sizeof(metadata));     // *** ***

    }
}


/// m61_realloc(ptr, sz, file, line)
///    Reallocate the dynamic memory pointed to by `ptr` to hold at least
///    `sz` bytes, returning a pointer to the new block. If `ptr` is NULL,
///    behaves like `m61_malloc(sz, file, line)`. If `sz` is 0, behaves
///    like `m61_free(ptr, file, line)`. The allocation request was at
///    location `file`:`line`.

void* m61_realloc(void* ptr, size_t sz, const char* file, int line) {
    void* new_ptr = NULL;
    if (sz) {
        new_ptr = m61_malloc(sz, file, line);
    }
    if (ptr && new_ptr) {
        // Copy the data from `ptr` into `new_ptr`.
        // To do that, we must figure out the size of allocation `ptr`.
        // Your code here (to fix test014).
    }
    m61_free(ptr, file, line);
    return new_ptr;
}


/// m61_calloc(nmemb, sz, file, line)
///    Return a pointer to newly-allocated dynamic memory big enough to
///    hold an array of `nmemb` elements of `sz` bytes each. The memory
///    is initialized to zero. If `sz == 0`, then m61_malloc may
///    either return NULL or a unique, newly-allocated pointer value.
///    The allocation request was at location `file`:`line`.

void* m61_calloc(size_t nmemb, size_t sz, const char* file, int line) {
    // Your code here (to fix test016).
    void* ptr = m61_malloc(nmemb * sz, file, line);
    if (ptr) {
        memset(ptr, 0, nmemb * sz);
    }
    return ptr;
}


/// m61_getstatistics(stats)
///    Store the current memory statistics in `*stats`.

void m61_getstatistics(struct m61_statistics* stats) {
    // Stub: set all statistics to enormous numbers
    // memset(stats, 255, sizeof(struct m61_statistics)); // ---> why do memset are set to 255? B/c it is filled with something
    // Your code here.
     // printf("%s%lld\t%s%lld\t%s%lld\t%s%lld\t%s%lld\t%s%lld\t", "statistics size",statistics.active_size ,"statistics nactive",statistics.nactive , "statistics ntotal",statistics.ntotal, "statistics total_size",statistics.total_size, "statistics nfail",statistics.nfail , "statistics fail_size",statistics.fail_size );

    memcpy(&stats->nactive, &statistics.nactive, sizeof(stats->nactive)); // *** This copies nactive. B/c stats is a pointer, use -> ***

    memcpy(&stats->active_size, &statistics.active_size, sizeof(stats->active_size));     // *** # bytes in active allocations ***

    memcpy(&stats->ntotal, &statistics.ntotal, sizeof(stats->ntotal)); // *** This copies nactive. B/c stats is a pointer, use -> ***

    memcpy(&stats->total_size, &statistics.total_size, sizeof(stats->total_size));     // *** copies # bytes in total allocations ***

    memcpy(&stats->nfail, &statistics.nfail, sizeof(stats->nfail));     // *** copies # failed allocation attempts ***

    memcpy(&stats->fail_size, &statistics.fail_size, sizeof(stats->fail_size));     // *** copies # bytes in failed alloc attempts ***

    memcpy(&stats->heap_min, &statistics.heap_min, sizeof(stats->heap_min));     // *** copies smallest allocated addr ***

    memcpy(&stats->heap_max, &statistics.heap_max, sizeof(stats->heap_max));     // *** copies largest allocated addr ***

    // printf("stats size%10llu   stats nactive%10llu     stats ntotal%10llu      stats total_size%10llu      stats nfail%10llu      stats fail_size%10llu\n", &stats->active_size ,&stats->nactive , &stats->ntotal, &stats->total_size, &stats->nfail , &stats->fail_size );

    // printf("malloc count: active %10llu   total %10llu   fail %10llu\n",
    //        stats->nactive, stats->ntotal, stats->nfail);
    // printf("malloc size:  active %10llu   total %10llu   fail %10llu\n",
    //        stats->active_size, stats->total_size, stats->fail_size);

}


/// m61_printstatistics()
///    Print the current memory statistics.

void m61_printstatistics(void) {
    struct m61_statistics stats;
    m61_getstatistics(&stats);

    printf("malloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("malloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}


/// m61_printleakreport()
///    Print a report of all currently-active allocated blocks of dynamic
///    memory.

void m61_printleakreport(void) {
    // Your code here.
}
