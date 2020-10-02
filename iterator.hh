#ifndef __ITERATOR_HH

template <typename T>
class Iterator {
    T *m_buffer;
    size_t m_buffer_length;
    T *m_position;

    Iterator() {}
public:
    /**
     * Returns an iterator pointing at the start of the buffer `buf'.
     */
    static Iterator<T> begin(T *buf, size_t buf_length) {
        Iterator<T> iterator;
        iterator.m_buffer = buf;
        iterator.m_buffer_length = buf_length;
        iterator.begin();
        return iterator;
    }

    /**
     * Returns an iterator pointing at the end of the buffer `buf'.
     */
    static Iterator<T> end(T *buf, size_t buf_length) {
        Iterator<T> iterator;
        iterator.m_buffer = buf;
        iterator.m_buffer_length = buf_length;
        iterator.end();
        return iterator;
    }

    inline void begin() {
        m_position = m_buffer;
    }

    inline void end() {
        m_position = m_buffer + m_buffer_length;
    }

    ~Iterator() {}

    inline Iterator<T> operator++() {
        m_position++;
        return *this;
    }

    inline Iterator<T> operator++(int _) {
        m_position++;
        return *this;
    }

    inline T &operator*() const {
        return *m_position;
    }

    inline T *operator->() const {
        return m_position;
    }

    inline bool operator==(const Iterator<T> &rhs) const {
        return m_position == rhs.m_position;
    }

    inline bool operator!=(const Iterator<T> &rhs) const {
        return m_position != rhs.m_position;
    }
};

#define __ITERATOR_HH
#endif
