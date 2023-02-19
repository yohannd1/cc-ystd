#ifndef _YSL_BUFFER_HH
#define _YSL_BUFFER_HH

#include "alloc.hh"
#include "iter.hh"
#include "math.hh"
#include "slice.hh"

#include <utility>

namespace ysl {
    namespace err {
        struct AllocError {};
    } // namespace err

    template <typename T, Allocator A = LibcAllocator>
    class Buffer {
        T* m_buffer;
        size_t m_element_len;
        size_t m_true_len;
        size_t m_reserved_len;

        /**
         * Reallocate the buffer, if needed.
         */
        void maybe_reallocate() {
            size_t new_len = max(m_reserved_len, m_element_len);

            if (m_true_len != new_len) {
                m_true_len = new_len;

                if (m_true_len == 0) {
                    // if it's 0, the previous length was greater (m_true_len != new_len)
                    // so we can free the buffer without worrying if it was null
                    A::free(m_buffer);
                    m_buffer = nullptr;
                } else {
                    if (m_buffer == nullptr) {
                        m_buffer = (T*) A::alloc(m_true_len * sizeof(T));
                        // TODO: null check
                    } else {
                        T* new_alloc = (T*) A::realloc(m_buffer, m_true_len * sizeof(T));

                        if (new_alloc == nullptr) {
                            throw err::AllocError {};
                        } else {
                            m_buffer = new_alloc;
                        }
                    }
                }
            }
        }

    public:
        /**
         * Create a buffer with a reserved minimal size.
         * Useful for preventing too many allocations.
         */
        static auto with_reserved_size(size_t size) -> Buffer<T, A> {
            Buffer<T, A> buffer;
            buffer.reserve(size);
            return buffer;
        }

        /**
         * Change the reserved size of a buffer.
         */
        void reserve(size_t len) {
            m_reserved_len += len;
            maybe_reallocate();
        }

        operator ysl::ConstSlice<T>() const { return ysl::MutSlice(m_buffer, m_element_len); }

        operator ysl::MutSlice<T>() { return ysl::MutSlice(m_buffer, m_element_len); }

        /**
         * Returns the true length currently allocated by this buffer.
         * Output might be different than the len() method if it's reserved.
         */
        inline size_t true_alloc_len() const { return m_true_len; }

        /**
         * Returns the amount of elements currently stored on this buffer.
         */
        inline size_t len() const { return m_element_len; }

        /**
         * The default constructor for a buffer.
         * By default (if it's not reserved), there is no allocation at creation.
         */
        Buffer() : m_buffer(nullptr), m_element_len(0), m_true_len(0), m_reserved_len(0) {}

        ~Buffer() {
            if (m_buffer != nullptr) {
                A::free(m_buffer);
            }
        }

        /**
         * Pushes an element onto the end of the buffer.
         */
        void push(T thing) {
            m_element_len++;
            maybe_reallocate();
            m_buffer[m_element_len - 1] = thing;
        }

        template <int n>
        static auto from(const T array[n]) -> Buffer<T, A> {
            auto buffer = Buffer<T, A>::with_reserved_size(n);
            for (size_t i = 0; i < n; i++) {
                buffer.push(array[i]);
            }
            return buffer;
        }

        static auto from_sized_array(const T array[], size_t size) -> Buffer<T, A> {
            auto buffer = Buffer<T, A>::with_reserved_size(size);
            for (size_t i = 0; i < size; i++) {
                buffer.push(array[i]);
            }

            return buffer;
        }

        /**
         * Pops an element off the buffer and returns it.
         */
        auto pop() -> T {
            if (m_element_len == 0) {
                throw err::IndexOutOfBounds {};
            } else {
                T thing = m_buffer[m_element_len - 1];
                m_element_len--;
                maybe_reallocate();

                return thing;
            }
        }

        /**
         * Checks if the specified index exists at this buffer.
         */
        inline auto inside_bounds(size_t index) const -> bool { return index < m_element_len; }

        auto operator[](size_t index) -> T& {
            if (inside_bounds(index)) {
                return m_buffer[index];
            } else {
                throw err::IndexOutOfBounds {};
            }
        }

        auto operator[](size_t index) const -> const T& {
            if (inside_bounds(index)) {
                return m_buffer[index];
            } else {
                throw err::IndexOutOfBounds {};
            }
        }

        auto get_or_null(size_t index) -> T* {
            if (inside_bounds(index)) {
                return &m_buffer[index];
            } else {
                throw err::IndexOutOfBounds {};
            }
        }

        auto get_or_null(size_t index) const -> const T* {
            if (inside_bounds(index)) {
                return &m_buffer[index];
            } else {
                throw err::IndexOutOfBounds {};
            }
        }

        inline auto mem_ptr_mut() -> T* { return m_buffer; }

        inline auto mem_ptr() const -> const T* { return m_buffer; }

        inline auto iter() -> ArrayIter<T> { return ArrayIter<T>::begin(m_buffer, m_element_len); }

        inline auto begin() -> ArrayIter<T> { return ArrayIter<T>::begin(m_buffer, m_element_len); }

        inline auto end() -> ArrayIter<T> { return ArrayIter<T>::end(m_buffer, m_element_len); }

        // TODO: auto clone() const -> Buffer<T, A> requires Clone<T> {}

        auto insert_at(size_t index, T element) {
            if (index > m_element_len) {
                throw err::IndexOutOfBounds {};
            } else {
                return; // TODO
            }
        }
    };
} // namespace ysl

#endif
