#ifndef _YSL_SHARED_HH
#define _YSL_SHARED_HH

#include <cstdlib>
#include "heap_tools.hh"

namespace ysl {
    template <typename T>
    class Shared {
    private:
        struct Inner {
            mutable size_t owner_count;
            T data;
        };

        ManualFree<Inner> heap;
    public:
        Shared() {
            this->heap->owner_count = 1;
        }

        Shared(T data) {
            this->heap.get_buf()->owner_count = 1;
            this->heap.get_buf()->data = data;
        }

        Shared(const Shared<T>& shared) {
            shared.heap.get_buf()->owner_count += 1;
            this->heap = shared.heap.shallow_copy();
        }

        auto operator&() const -> const T* { return &heap->data; }
        auto operator*() const -> const T& { return heap.get_buf()->data; }
    };

    template <typename T>
    Shared<T> own(const Shared<T> &shared) {
        return shared;
    }
}

#endif
