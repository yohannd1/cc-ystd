#ifndef _YSL_HEAP_TOOLS
#define _YSL_HEAP_TOOLS

#include "alloc.hh"

namespace ysl {
    template <typename T, Allocator<T> Alloc = StandardAllocator<T>>
    class ManualFree {
    private:
        T *buf;

        explicit ManualFree(T *buf): buf(buf) {}
    public:
        ManualFree(const ManualFree<T, Alloc>&) = delete;

        ManualFree() {
            this->buf = Alloc::alloc(1);
            *this->buf = T();
        }

        ManualFree(T data) {
            this->buf = Alloc::alloc(1);
            *this->buf = data;
        }

        static auto from_buf(T *buf) -> ManualFree<T, Alloc> {
            return ManualFree(buf);
        }

        auto operator&() -> T* { return this->buf; }
        auto operator&() const -> const T* { return this->buf; }
        auto operator*() -> T& { return *this->buf; }
        auto operator*() const -> const T& { return *this->buf; }

        auto get_buf() const -> const T* { return this->buf; }
        auto get_buf() -> T* { return this->buf; }

        auto shallow_copy() const -> ManualFree<T, Alloc> {
            return ManualFree { .buf = buf };
        }

        // TODO: auto deep_copy() const -> ManualFree<T, Alloc>;
    };
}

#endif
