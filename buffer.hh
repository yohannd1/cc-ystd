#ifndef _BUFFER_HH
#define _BUFFER_HH

#include <utility>

#include "math.hh"
#include "iter.hh"
#include "alloc.hh"

template <typename T, Allocator<T> A = StandardAllocator<T>>
class Buffer {
    T *m_buffer;
    size_t m_element_len;
    size_t m_true_len;
    size_t m_reserved_len;

    /**
     * Reallocate buffer, if needed.
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
                    m_buffer = A::alloc(m_true_len);
                    // TODO: null check
                } else {
                    m_buffer = A::realloc(m_buffer, m_true_len * sizeof (T));
                    // m_buffer = (T*) Allocator::realloc(m_buffer, m_true_len * sizeof (T));
                    // TODO: null check
                }
            }
        }
    }
public:
    /**
     * Create a buffer with a reserved minimal size.
     * Useful for preventing too many allocations.
     */
    static Buffer<T> with_reserved_size(size_t size) {
        Buffer<T> buffer;
        buffer.reserve(size);
        return buffer;
    }

    /**
     * Change the reserved size of a buffer.
     */
    void reserve(size_t len) {
        m_reserved_len = len;
        maybe_reallocate();
    }

    /**
     * Returns the true length currently allocated by this buffer.
     * Output might be different than the len() method if it's reserved.
     */
    inline size_t true_alloc_len() const {
        return m_true_len;
    }

    /**
     * Returns the amount of elements currently stored on this buffer.
     */
    inline size_t len() const {
        return m_element_len;
    }

    /**
     * The default constructor for a buffer.
     * By default (if it's not reserved), there is no allocation at creation.
     */
    Buffer() :
        m_buffer(nullptr),
        m_element_len(0),
        m_true_len(0),
        m_reserved_len(0)
    {}

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
    static Buffer<T> from(const T array[n]) {
        auto buffer = Buffer<T>::with_reserved_size(n);
        for (size_t i = 0; i < n; i++) {
            buffer.push(array[i]);
        }
        return buffer;
    }

    static Buffer<T> from_sized_array(const T array[], size_t size) {
        auto buffer = Buffer<T>::with_reserved_size(size);
        for (size_t i = 0; i < size; i++) {
            buffer.push(array[i]);
        }

        return buffer;
    }

    /**
     * Pops an element off the buffer and returns it.
     */
    T pop() {
        if (m_element_len == 0) {
            throw "out of bounds";
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
    inline bool inside_bounds(size_t index) const {
        return index < m_element_len;
    }

    T &operator[](size_t index) {
        if (inside_bounds(index)) {
            T *element = &m_buffer[index];
            return *element;
        } else {
            throw "out of bounds";
        }
    }

    inline T *mem_ptr_mut() {
        return m_buffer;
    }

    inline const T *mem_ptr() const {
        return m_buffer;
    }

    ArrayIter<T> begin() {
        return ArrayIter<T>::begin(m_buffer, m_element_len);
    }

    ArrayIter<T> end() {
        return ArrayIter<T>::end(m_buffer, m_element_len);
    }

    // TODO: ArrayIter<T> iter() {}
    // TODO: Buffer clone() const
    // TODO: T &operator[](size_t index)
    // TODO: T *get_or_null(size_t index)
    // TODO: insert_at(T thing, size_t index)
};

#endif
