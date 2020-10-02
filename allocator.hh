#ifndef _ALLOCATOR_HH
#define _ALLOCATOR_HH

#include <cstdlib>

class Allocator {
public:
    static void *alloc(size_t size);
    static void *realloc(void *buffer, size_t new_size);
    static void free(void *buffer);
};

class StandardAllocator {
public:
    static void *alloc(size_t size) {
        return std::malloc(size);
    }

    static void *realloc(void *buffer, size_t new_size) {
        return std::realloc(buffer, new_size);
    }

    static void free(void *buffer) {
        std::free(buffer);
    }
};

template <typename T, typename Allocator>
static T *alloc_elements(size_t amount) {
    return (T*) Allocator::alloc(amount * sizeof (T));
}

template <typename T, typename Allocator>
static T *realloc_elements(T *buf, size_t new_amount) {
    return (T*) Allocator::realloc(buf, new_amount * sizeof (T));
}

#endif
