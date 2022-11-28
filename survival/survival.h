#ifndef _SURVIVAL_H_
#define _SURVIVAL_H_

/// \defgroup basic functions
/// \{

/// Minimum of a and b (must be comparable).
#define MIN(a, b) ((a) > (b)) ? (b) : (a)

/// Maximum of a and b (must be comparable).
#define MAX(a, b) ((a) < (b)) ? (b) : (a)

/// Absolute value of x
#define ABS(x) (((x) < 0) ? -(x) : (x))

/// Returns a non-zero value if x is a power of two.
#define is_power_of_two(x) ((x) && !((x) & ((x)-1)))

/// Retrieve the number of items in a array.
#define COUNT_OF(x)                                                            \
    ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

/// \}

/// \defgroup minimal logging
/// \{

/// Supported log-levels, you can easily add a new one if needed.
enum
{
    SURVIVAL_LOG_DEBUG,
    SURVIVAL_LOG_ERROR
};

#ifndef SURVIVAL_LOG_BASENAME_ONLY
/// If non-zero, only the basename of source files will be included (more
/// readable logs).
#define SURVIVAL_LOG_BASENAME_ONLY 1
#endif

#ifdef SURVIVAL_DEBUG
#define EPRINTF(...)                                                           \
    do {                                                                       \
        survival_log(                                                          \
            SURVIVAL_LOG_ERROR, __FILE__, __func__, __LINE__, __VA_ARGS__);    \
    } while (0)
#define DPRINTF(...)                                                           \
    do {                                                                       \
        survival_log(                                                          \
            SURVIVAL_LOG_DEBUG, __FILE__, __func__, __LINE__, __VA_ARGS__);    \
    } while (0)
#else
// When SURVIVAL_DEBUG is false, we keep only EPRINT with a minimal format (no
// file and function).
#define EPRINTF(...)                                                           \
    do {                                                                       \
        survival_log(SURVIVAL_LOG_ERROR, "", "", __LINE__, __VA_ARGS__);       \
    } while (0)
#define DPRINTF(fmt, args...)
#endif

/// Callbacks invoked by logging macros (EPRINTF, DPRINTF).
int
survival_log(
    long level,
    const char* file,
    const char* func,
    long line,
    const char* fmt,
    ...) __attribute__((format(printf, 5, 6)));

/// \}

#if defined(__GNUC__) || defined(__clang__)
/// Indicate to the compiler that a given predicate is
/// very likely to be true.
#define LIKELY(x) __builtin_expect((x), 1)

/// Indicate to the compiler that a given predicate is
/// very likely to be false.
#define UNLIKELY(x) __builtin_expect((x), 0)

/// Specify alignment of structures or variables.
#define ALIGNED(x) __attribute__((aligned(x)))

#define ASSUME(cond)                                                           \
    do {                                                                       \
        if (!(cond))                                                           \
            __builtin_unreachable();                                           \
    } while (0)

/// Define a scope cleanup function.
#define SURVIVAL_CLEANUP(func) __attribute__((cleanup(func)))

#else
#error "Only clang or GCC are supported currently."
#endif // __clang__, __GNUC__

#define DEFINE_TRIVIAL_CLEANUP_FUNC(name, type, func)                          \
    static void name(type* p)                                                  \
    {                                                                          \
        if (*p) {                                                              \
            func(*p);                                                          \
        }                                                                      \
    }                                                                          \
    struct __useless_struct_to_allow_trailing_semicolon__

#define DEFINE_CLEANUP_FREE()                                                  \
    static void cleanup_free(void* p) { free(*(void**)p); }                    \
    struct __useless_struct_to_allow_trailing_semicolon__

#endif
