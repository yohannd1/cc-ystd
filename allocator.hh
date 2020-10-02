#ifndef _ALLOCATOR_HH

#include <cstdlib>

class Allocator {
public:
    static void *allocate_bytes(size_t size);
    static void *reallocate_bytes(void *buffer, size_t new_size);

    template <typename T>
    static T *allocate_elements(size_t amount);

    template <typename T>
    static T *reallocate_elements(T *buffer, size_t new_amount);

    static void free(void *buffer);
};

class StandardAllocator {
public:
    static void *allocate_bytes(size_t size) {
        return malloc(size);
    }

    static void *reallocate_bytes(void *buffer, size_t new_size) {
        return realloc(buffer, new_size);
    }

    template <typename T>
    static T *allocate_elements(size_t amount) {
        return malloc(amount * sizeof (T));
    }

    template <typename T>
    static T *reallocate_elements(T *buffer, size_t new_amount) {
        return realloc(buffer, new_amount * sizeof (T));
    }

    static void free(void *buffer) {
        free(buffer);
    }
};

#define _ALLOCATOR_HH
#endif
