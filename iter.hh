#ifndef _ITERATOR_HH
#define _ITERATOR_HH

#include <concepts>

template <typename Self, typename Output>
concept Iterator = requires(Self self, Self other) {
    // static methods
    { Self::begin() } -> std::same_as<Self>;
    { Self::end() } -> std::same_as<Self>;

    // iterator methods
    { self.begin() } -> std::same_as<void>;
    { self.end() } -> std::same_as<void>;
    { self++ } -> std::same_as<void>;
    { self-- } -> std::same_as<void>;
    { *self } -> std::same_as<Output&>;
    { &self } -> std::same_as<Output*>;
    { self.operator->() } -> std::same_as<Output*>;
    { self == other } -> std::same_as<bool>;
    { self != other } -> std::same_as<bool>;
};

template <typename T>
class ArrayIter {
    T *m_buffer;
    size_t m_buffer_length;
    T *m_position;

    ArrayIter() {}
public:
    /**
     * Returns an iterator pointing at the start of the buffer `buf'.
     */
    static ArrayIter<T> begin(T *buf, size_t buf_length) {
        ArrayIter<T> iterator;
        iterator.m_buffer = buf;
        iterator.m_buffer_length = buf_length;
        iterator.begin();
        return iterator;
    }

    /**
     * Returns an iterator pointing at the end of the buffer `buf'.
     */
    static ArrayIter<T> end(T *buf, size_t buf_length) {
        ArrayIter<T> iterator;
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

    ~ArrayIter() {}

    inline void operator++() {
        m_position++;
    }

    inline void operator++(int _) {
        m_position++;
    }

    inline T &operator*() const {
        return *m_position;
    }

    inline T *operator->() const {
        return m_position;
    }

    inline bool operator==(const ArrayIter<T> &rhs) const {
        return m_position == rhs.m_position;
    }

    inline bool operator!=(const ArrayIter<T> &rhs) const {
        return m_position != rhs.m_position;
    }
};

#endif
