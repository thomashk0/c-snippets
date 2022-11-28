#include "survival.h"

#include <stdarg.h>
#include <stdio.h>

static char const* const level_str
    [] = { [SURVIVAL_LOG_DEBUG] = "debug", [SURVIVAL_LOG_ERROR] = "error" };

static char const*
log_basename(char const* f_name)
{
    if (f_name == NULL) {
        return NULL;
    }
    long offset = 0;
    long i = 0;
    for (;;) {
        char c = f_name[i];
        if (c == 0) {
            break;
        }
        if (c == '/') {
            offset = i + 1;
        }
        i++;
    }
    return f_name + offset;
}

int
survival_log(
    long level,
    const char* file,
    const char* func,
    long line,
    const char* fmt,
    ...)
{
    if (level < 0 || level >= (long)COUNT_OF(level_str) ||
        level_str[level] == NULL) {
        return -1;
    }
    char const* f_name = SURVIVAL_LOG_BASENAME_ONLY ? log_basename(file) : file;
    va_list arglist;
    fprintf(
        stderr, "[%s](%s:%ld in %s) ", level_str[level], f_name, line, func);
    va_start(arglist, fmt);
    vfprintf(stderr, fmt, arglist);
    va_end(arglist);
    return 0;
}
