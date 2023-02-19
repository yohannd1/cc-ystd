#ifndef _YSL_ALLOC_HH
#define _YSL_ALLOC_HH

#include <concepts>
#include <cstdlib>

namespace ysl {
    template <typename Self>
    concept Allocator = requires(size_t size, void* buffer) {
        { Self::alloc(size) } -> std::same_as<void*>;
        { Self::realloc(buffer, size) } -> std::same_as<void*>;
        { Self::free(buffer) } -> std::same_as<void>;
    };

    struct LibcAllocator {
        static auto alloc(size_t size) -> void* { return std::malloc(size); }
        static auto realloc(void* buffer, size_t new_size) -> void* {
            return std::realloc(buffer, new_size);
        }
        static auto free(void* buffer) -> void { std::free(buffer); }
    };

    template <typename T, Allocator Alloc = LibcAllocator>
    class ManualFree {
    private:
        T* buf_ptr;

        explicit ManualFree(T* buf) : buf_ptr(buf) {}

    public:
        ManualFree() = delete;

        explicit ManualFree(const ManualFree<T, Alloc>& other) { buf_ptr = other.buf_ptr; }

        static auto from_buf(T* buf) -> ManualFree<T, Alloc> { return ManualFree(buf); }

        // TODO: better name
        static auto with_element(T element) -> ManualFree<T, Alloc> {
            T* buf = (T*) Alloc::alloc(sizeof(T));
            *buf = element;

            return ManualFree<T, Alloc>::from_buf(buf);
        }

        auto shallow_copy() const -> ManualFree<T, Alloc> {
            return ManualFree::from_buf(this->buf_ptr);
        }

        auto shallow_free() { Alloc::free(this->buf_ptr); }

        // TODO: auto deep_copy() const -> ManualFree<T, Alloc>;

        inline auto as_ptr() -> T* { return this->buf_ptr; }
        inline auto as_ptr() const -> const T* { return this->buf_ptr; }
        inline auto as_ref() -> T& { return *this->buf_ptr; }
        inline auto as_ref() const -> const T& { return *this->buf_ptr; }

        inline auto operator->() -> T* { return this->buf_ptr; }
        inline auto operator->() const -> const T* { return this->buf_ptr; }
        inline auto operator&() -> T* { return this->buf_ptr; }
        inline auto operator&() const -> const T* { return this->buf_ptr; }
        inline auto operator*() -> T& { return *this->buf_ptr; }
        inline auto operator*() const -> const T& { return *this->buf_ptr; }
    };
} // namespace ysl

#endif
