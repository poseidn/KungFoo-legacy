#pragma once

#define GCC_LIKELY(x)    __builtin_expect ( x, 1)
#define GCC_UNLIKELY(x)  __builtin_expect ( x, 0)

