#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifndef CORROSION_DISABLE_ASSERT
#include <cassert>
#define CORROSION_ASSERT(condition) assert(condition)
#else
#define CORROSION_ASSERT(...) ((void)0)
#endif

#ifndef CORROSION_DISABLE_NOEXCEPT
#define CORROSION_NOEXCEPT noexcept
#else
#define CORROSION_NOEXCEPT
#endif

#ifndef CORROSION_NO_DEBUG_LOOP
#define CORROSION_DEBUG_LOOP true
#else
#define CORROSION_DEBUG_LOOP false
#endif

#endif // CONFIG_HPP
