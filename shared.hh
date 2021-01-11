#ifndef _YSL_SHARED_HH
#define _YSL_SHARED_HH

#include <cstdlib>
#include "alloc.hh"

namespace ysl {
    template <typename T>
    struct _SharedInner {
        mutable size_t owner_count;
        T data;
    };

    template <typename T, Allocator Alloc = LibcAllocator>
    class Shared {
    private:
        ManualFree<_SharedInner<T>, Alloc> shared_buf;
    public:
        Shared(): shared_buf(
            ManualFree<T, Alloc>::with_element(_SharedInner {
                .owner_count = 1,
                .data = T() }))
        {}

        Shared(T data): shared_buf(
            ManualFree<_SharedInner<T>, Alloc>::with_element(_SharedInner {
                .owner_count = 1,
                .data = data }))
        {}

        Shared(const Shared<T, Alloc>& shared):
            shared_buf(shared.shared_buf.shallow_copy())
        {
            this->shared_buf->owner_count += 1;
        }

        auto operator&() const -> const T* { return &this->shared_buf->data; }
        auto operator*() const -> const T& { return this->shared_buf->data; }
    };

    template <typename T, Allocator Alloc = LibcAllocator>
    Shared<T, Alloc> own(const Shared<T> &shared) {
        return shared;
    }
}

#endif
