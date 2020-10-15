#ifndef _VARIANT_HH
#define _VARIANT_HH

#include "exception.hh"

template <typename T>
class Maybe {
    union Data {
        char nothing; // TODO: find something better
        T content;
    };

    bool m_is_some;
    Data m_content;

    Maybe(T content)
        : m_is_some(true), m_content({ .content = content }) {}
    Maybe()
        : m_is_some(false), m_content({ .nothing = '\0' }) {}
public:
    static Maybe<T> some(T content) {
        return Maybe<T>(content);
    }

    static Maybe<T> none() {
        return Maybe<T>();
    }

    inline bool is_some() const {
        return m_is_some;
    }

    T unwrap() const {
        if (m_is_some) {
            return m_content.content;
        } else {
            throw Exception("failed to unwrap a Maybe class");
        }
    }

    const T *inside_ptr() const {
        if (m_is_some) {
            return &m_content.content;
        } else {
            throw Exception("failed to unwrap a Maybe class");
        }
    }

    T *inside_ptr_mut() {
        if (m_is_some) {
            return &m_content.content;
        } else {
            throw Exception("failed to unwrap a Maybe class");
        }
    }
};

template <typename T, typename E>
class Either {
    union Data {
        T left;
        E right;
    };

    enum class Side {
        Left,
        Right,
    };

    Side m_side;
    Data m_data;

    Either() = default;
public:
    static Either<T, E> left(T left) {
        Either<T, E> either;
        either.m_side = Side::Left;
        either.m_data.left = left;
        return either;
    }

    static Either<T, E> right(E right) {
        Either<T, E> either;
        either.m_side = Side::Right;
        either.m_data.right = right;
        return either;
    }

    inline bool is_left() const {
        return m_side == Side::Left;
    }

    inline bool is_right() const {
        return m_side == Side::Right;
    }

    T unwrap_left() const {
        if (is_left()) {
            return m_data.left;
        } else {
            throw Exception("Either class: tried to get right value when value was on left side");
        }
    }

    T unwrap_right() const {
        if (is_right()) {
            return m_data.right;
        } else {
            throw Exception("Either class: tried to get left value when value was on right side");
        }
    }

    const T *left_ptr() const {
        if (is_left()) {
            return &m_data.left;
        } else {
            throw Exception("Either class: tried to get right value when value was on left side");
        }
    }

    T *left_ptr_mut() {
        if (is_left()) {
            return &m_data.left;
        } else {
            throw Exception("Either class: tried to get right value when value was on left side");
        }
    }

    const T *right_ptr() const {
        if (is_right()) {
            return &m_data.right;
        } else {
            throw Exception("Either class: tried to get right value when value was on left side");
        }
    }

    T *right_ptr_mut() {
        if (is_right()) {
            return &m_data.right;
        } else {
            throw Exception("Either class: tried to get right value when value was on left side");
        }
    }
};

#endif
