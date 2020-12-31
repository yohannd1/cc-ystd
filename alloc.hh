#ifndef _ALLOCATOR_HH
#define _ALLOCATOR_HH

#include <cstdlib>
#include <concepts>

template <typename Self, typename T>
concept Allocator = requires(size_t size, T *buffer) {
    { Self::alloc(size) } -> std::same_as<T*>;
    { Self::realloc(buffer, size) } -> std::same_as<T*>;
    { Self::free(buffer) } -> std::same_as<void>;
};

template <typename T>
struct StandardAllocator {
    static T *alloc(size_t size) {
        return (T*) std::malloc(size);
    }

    static T *realloc(void *buffer, size_t new_size) {
        return (T*) std::realloc(buffer, new_size);
    }

    static void free(void *buffer) {
        std::free(buffer);
    }
};

template <typename T, Allocator<T> A>
static T *alloc_elements(size_t amount) {
    return (T*) A::alloc(amount * sizeof (T));
}

template <typename T, Allocator<T> A>
static T *realloc_elements(T *buf, size_t new_amount) {
    return (T*) A::realloc(buf, new_amount * sizeof (T));
}

#endif
