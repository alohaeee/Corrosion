#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifndef RUSTC_DISABLE_ASSERT
#include <cassert>
#define RUSTC_ASSERT(condition) assert(condition)
#else
#define RUSTC_ASSER(...) ((void)0)
#endif

#endif // CONFIG_HPP
