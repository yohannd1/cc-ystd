#ifndef _STRING_HH
#define _STRING_HH

#include "buffer.hh"

template <typename C = char>
class GenericString {
    Buffer<C> m_buffer;
    bool m_has_null_term;
public:
    GenericString() : m_has_null_term(false) {
        m_buffer = Buffer<C>();
    }

    template <size_t n>
    static GenericString<C> from_sized(const C array[n]) {
        GenericString<C> string;
        string.push_sized<n>(array);
        return string;
    }

    static GenericString<C> from_str(const C *str) {
        GenericString<C> string;
        string.push_str(str);
        return string;
    }

    void push_str(const C *array) {
        // remove null termination if there's any
        if (m_has_null_term) {
            m_buffer.pop();
        } else {
            m_has_null_term = true;
        }
        
        // push characters from the string until '\0' is found
        for (const C *ch = array; *ch; ch++) {
            // TODO: make a push_sized for buffer.hh so it doesn't reallocate for every char
            m_buffer.push(*ch);
        }

        m_buffer.push('\0');
    }

    void push_char(const C ch) {
        // remove null termination if there's any
        if (m_has_null_term) {
            m_buffer.pop();
        } else {
            m_has_null_term = true;
        }

        m_buffer.push(ch);
        m_buffer.push('\0');
    }

    template <size_t n>
    void push_sized(const C array[n]) {
        // remove null termination if there's any
        if (m_has_null_term) {
            m_buffer.pop();
        } else {
            m_has_null_term = true;
        }

        // push a fixed amount of chars
        for (size_t i = 0; i < n; i++) {
            m_buffer.push(array[i]);
        }

        m_buffer.push('\0');
    }

    inline size_t len() const {
        return m_has_null_term ? m_buffer.len() - 1 : 0;
    }
    
    inline size_t len_with_null() const {
        return m_buffer.len();
    }

    const C *as_str() const {
        return m_buffer.mem_ptr();
    }

    C *as_str_mut() {
        return m_buffer.mem_ptr_mut();
    }
};

using String = GenericString<char>;

#endif
