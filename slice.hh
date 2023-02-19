#ifndef _YSL_SLICE_HH
#define _YSL_SLICE_HH

namespace ysl {
    namespace err {
        struct IndexOutOfBounds {};
    } // namespace err

    template <typename T>
    class ConstSlice {
    private:
        const T *m_buf;
        size_t m_len;

    public:
        inline auto len() const -> size_t { return m_len; }

        inline auto as_ptr() const -> const T * { return m_buf; }

        inline auto operator[](size_t idx) const -> const T & {
            if (idx >= m_len) {
                throw err::IndexOutOfBounds {};
            } else {
                return this->as_ptr()[idx];
            }
        }

        inline auto operator&() const -> const T * { return this->as_ptr(); }
    };

    template <typename T>
    class MutSlice {
    private:
        T *const buf;
        const size_t size;

    public:
        inline operator ConstSlice<T>() const noexcept {
            return ConstSlice<T>(this->buf, this->size);
        }

        inline auto len() const -> size_t { return this->size; }

        inline auto as_ptr() const -> const T * { return this->buf; }
        inline auto as_ptr() -> T * { return this->buf; }

        inline auto operator[](size_t idx) const -> const T & {
            if (idx >= this->size) {
                throw err::IndexOutOfBounds {};
            } else {
                return this->as_ptr()[idx];
            }
        }

        inline auto operator[](size_t idx) -> T & {
            if (idx >= this->size) {
                throw err::IndexOutOfBounds {};
            } else {
                return this->as_ptr()[idx];
            }
        }

        inline auto operator&() const -> const T * { return this->as_ptr(); }
        inline auto operator&() -> T * { return this->as_ptr(); }
    };
} // namespace ysl

#endif
