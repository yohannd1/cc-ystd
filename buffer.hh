#ifndef _BUFFER_HH

#include <cstdlib>
#include <utility>

#include "iterator.hh"
// TODO: #include "allocator.hh"

template <typename T>
class Buffer {
    T *m_buffer;
    size_t m_true_len;
    size_t m_element_len;
    size_t m_reserved_len;

    /**
     * Returns the greater of two numbers.
     */
    template <typename N>
    static N max(N x, N y) {
        return x > y ? x : y;
    }

    /**
     * Reallocate buffer, if needed.
     */
    void maybe_reallocate() {
        size_t new_len = max<size_t>(m_reserved_len, m_element_len);

        if (m_true_len != new_len) {
            m_true_len = new_len;
            if (m_true_len == 0) {
                // if it's 0, the previous length was greater so we can free the buffer
                // without worrying if it was null
                delete[] m_buffer;
                m_buffer = nullptr;
            } else {
                if (m_buffer == nullptr) {
                    m_buffer = new T[m_true_len];
                    // TODO: null check
                } else {
                    m_buffer = (T*) realloc(m_buffer, m_true_len * sizeof (T));
                    // TODO: null check
                }
            }
        }
    }
public:
    /**
     * Copying shouldn't be implicit.
     */
    Buffer(const Buffer<T>&) = delete;

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
    Buffer() {
        m_buffer = nullptr;
        m_element_len = 0;
        m_true_len = 0;
    }

    ~Buffer() {
        if (m_buffer != nullptr) {
            delete[] m_buffer;
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

    /**
     * Pops an element off the buffer and returns it.
     */
    T pop(T thing) {
        if (m_element_len == 0) {
            // TODO: die
        } else {
            T thing = std::move(m_buffer[m_element_len - 1]);
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
            // TODO: die
            T foo;
            return foo;
        }
    }

    Iterator<T> begin() {
        return Iterator<T>::begin(m_buffer, m_element_len);
    }

    Iterator<T> end() {
        return Iterator<T>::end(m_buffer, m_element_len);
    }

    // TODO: Iterator<T> iter() {}
    // TODO: Buffer clone() const
    // TODO: T &operator[](size_t index)
    // TODO: T *get_or_null(size_t index)
    // TODO: insert_at(T thing, size_t index)
};

#define _BUFFER_HH
#endif
