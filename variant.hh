#ifndef _VARIANT_HH
#define _VARIANT_HH

#include "types.hh"
#include "exception.hh"

template <typename T>
class Maybe {
    union Data {
        u8 nothing;
        T data;

        ~Data() = delete;
    };

    bool m_is_some;
    Data m_data;

    Maybe(T data)
        : m_is_some(true), m_data({ .data = data }) {}
    Maybe()
        : m_is_some(false), m_data({ .nothing = '\0' }) {}
public:
    static Maybe<T> some(T data) {
        return Maybe<T>(data);
    }

    static Maybe<T> none() {
        return Maybe<T>();
    }

    ~Maybe() {
        if (is_some()) {
            m_data.~T();
        }
    }

    inline bool is_some() const {
        return m_is_some;
    }

    T unwrap() const {
        if (is_some()) {
            return m_data.data;
        } else {
            throw Exception("failed to unwrap a Maybe class");
        }
    }

    const T *inside_ptr() const {
        if (is_some()) {
            return &m_data.data;
        } else {
            throw Exception("failed to unwrap a Maybe class");
        }
    }

    T *inside_ptr_mut() {
        if (is_some()) {
            return &m_data.data;
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

        ~Data() {}
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

    ~Either() {
        if (is_left()) {
            m_data.left.~T();
        } else {
            m_data.right.~E();
        }
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
