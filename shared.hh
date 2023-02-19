#ifndef _YSL_SHARED_HH
#define _YSL_SHARED_HH

#include "alloc.hh"

#include <cstdlib>

namespace ysl {
    namespace err {
        struct SharedUnavailable {};
    } // namespace err

    template <typename T>
    struct _SharedHeapLayout {
        size_t strong_count;
        size_t weak_count;
        T data;
    };

    template <typename T, Allocator Alloc = LibcAllocator>
    class Weak;

    template <typename T, Allocator Alloc = LibcAllocator>
    class Shared {
    private:
        using HeapLayout = ManualFree<_SharedHeapLayout<T>, Alloc>;
        mutable HeapLayout heap_layout;

        explicit Shared(HeapLayout ly) : heap_layout(HeapLayout(ly)) {}

    public:
        Shared() = delete;

        Shared(T data) {
            heap_layout = HeapLayout::with_element(_SharedHeapLayout {
                .strong_count = 1,
                .weak_count = 0,
                .data = data,
            });
        }

        Shared(T&& data) {
            heap_layout = HeapLayout::with_element(_SharedHeapLayout {
                .strong_count = 1,
                .weak_count = 0,
                .data = std::move(data),
            });
        }

        Shared(const Shared<T, Alloc>& shared) = delete;

        ~Shared() {
            heap_layout->strong_count -= 1;

            if (heap_layout->strong_count == 0 && heap_layout->weak_count == 0) {
                heap_layout->data.~T();
                heap_layout.shallow_free();
            }
        }

        static auto from(T data) -> Shared<T, Alloc> {
            return Shared(HeapLayout::with_element(_SharedHeapLayout {
                .strong_count = 1,
                .weak_count = 0,
                .data = data,
            }));
        }

        auto to_share() const -> Shared<T, Alloc> {
            this->heap_layout->strong_count += 1;

            return Shared(this->heap_layout.shallow_copy());
        }

        auto to_weak() const -> Weak<T, Alloc> {
            return Weak<T, Alloc>::from_layout(this->heap_layout.shallow_copy());
        }

        static auto from_layout(HeapLayout layout) -> Shared<T, Alloc> {
            return Shared { .heap_layout = layout };
        }

        auto operator&() const -> const T* { return &this->heap_layout->data; }
        auto operator*() const -> const T& { return this->heap_layout->data; }
    };

    template <typename T, Allocator Alloc>
    class Weak {
    private:
        using HeapLayout = ManualFree<_SharedHeapLayout<T>, Alloc>;
        mutable HeapLayout heap_layout;

        Weak(HeapLayout ly) : heap_layout(HeapLayout(ly)) {}

    public:
        Weak() = delete;
        Weak(const Shared<T, Alloc>& weak) = delete;

        ~Weak() {
            heap_layout->weak_count -= 1;

            if (heap_layout->strong_count == 0 && heap_layout->weak_count == 0) {
                heap_layout->data.~T();
                heap_layout.shallow_free();
            }
        }

        static auto from_layout(HeapLayout ly) -> Weak<T, Alloc> {
            // FIXME: what thef ck did I do here? I'm not sure...
            return Weak(HeapLayout::from_buf(ly.as_ptr()));
        }

        auto try_upgrade() const -> Shared<T, Alloc> {
            if (this->heap_layout->stroung_count == 0) {
                throw err::SharedUnavailable {};
            } else {
                this->heap_layout->strong_count += 1;
                return Shared<T, Alloc>::from_heap_layout(this->heap_layout);
            }
        }

        auto operator&() const -> const T* { return &this->heap_layout->data; }
        auto operator*() const -> const T& { return this->heap_layout->data; }
    };
} // namespace ysl

#endif
