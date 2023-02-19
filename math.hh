#ifndef _YSL_MATH_HH
#define _YSL_MATH_HH

namespace ysl {
    template <typename N>
    static auto max(N x, N y) -> N {
        return x > y ? x : y;
    }

    template <typename N>
    static auto min(N x, N y) -> N {
        return x < y ? x : y;
    }
} // namespace ysl

#endif
