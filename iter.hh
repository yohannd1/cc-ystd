#ifndef _YSL_ITERATOR_HH
#define _YSL_ITERATOR_HH

#include <concepts>

namespace ysl {
    template <typename Self, typename Output>
    concept Iterator = requires(Self self, Self other) {
        // static methods
        { Self::begin() } ->std::same_as<Self>;
        { Self::end() } ->std::same_as<Self>;

        // iterator methods
        { self.begin() } ->std::same_as<void>;
        { self.end() } ->std::same_as<void>;
        { self++ } ->std::same_as<void>;
        { self-- } ->std::same_as<void>;
        { *self } ->std::same_as<Output &>;
        { &self } ->std::same_as<Output *>;
        { self.operator->() } ->std::same_as<Output *>;
        { self == other } ->std::same_as<bool>;
        { self != other } ->std::same_as<bool>;
    };

    template <typename T>
    class ArrayIter {
    private:
        T *m_buffer;
        size_t m_buffer_length;
        T *m_position;

        ArrayIter() {}

    public:
        /**
         * Returns an iterator pointing at the start of the buffer `buf'.
         */
        static auto begin(T *buf, size_t buf_length) -> ArrayIter<T> {
            ArrayIter<T> iterator;

            iterator.m_buffer = buf;
            iterator.m_buffer_length = buf_length;
            iterator.begin();

            return iterator;
        }

        /**
         * Returns an iterator pointing at the end of the buffer `buf'.
         */
        static auto end(T *buf, size_t buf_length) -> ArrayIter<T> {
            ArrayIter<T> iterator;

            iterator.m_buffer = buf;
            iterator.m_buffer_length = buf_length;
            iterator.end();

            return iterator;
        }

        inline auto begin() -> void { m_position = m_buffer; }

        inline auto end() -> void { m_position = m_buffer + m_buffer_length; }

        inline auto operator++() -> void { m_position++; }

        inline auto operator++(int _) -> void { m_position++; }

        inline auto operator*() -> T & { return *m_position; }

        inline auto operator->() -> T * { return m_position; }

        inline auto operator*() const -> const T & { return *m_position; }

        inline auto operator->() const -> const T * { return m_position; }

        inline auto operator==(const ArrayIter<T> &rhs) const -> bool {
            return m_position == rhs.m_position;
        }

        inline auto operator!=(const ArrayIter<T> &rhs) const -> bool {
            return m_position != rhs.m_position;
        }
    };
} // namespace ysl

using ysl::ArrayIter;
using ysl::Iterator;

#endif
