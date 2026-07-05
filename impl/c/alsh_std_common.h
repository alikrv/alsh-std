#ifndef ALSH_STD_COMMON_H
#define ALSH_STD_COMMON_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *alsh_strdup(const char *value);
char *alsh_vformat(const char *fmt, va_list args);
char *alsh_format(const char *fmt, ...);
char *alsh_array_to_string(char **items, size_t count);
char **alsh_parse_array(const char *value, size_t *count);
void alsh_free_array(char **items, size_t count);

#endif
