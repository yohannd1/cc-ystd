#ifndef _MATH_HH
#define _MATH_HH

template <typename N>
static N max(N x, N y) {
    return x > y ? x : y;
}

template <typename N>
static N min(N x, N y) {
    return x < y ? x : y;
}

#endif
