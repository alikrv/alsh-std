#include "alsh_std_common.h"

char *alsh_std_split(const char *input, const char *sep) {
    char *copy;
    char *result;
    size_t count = 0;
    char **items = NULL;
    size_t i;
    const char *cursor;
    const char *found;

    if (input == NULL) {
        return alsh_strdup("");
    }
    if (sep == NULL || *sep == '\0') {
        return alsh_strdup(input);
    }

    copy = alsh_strdup(input);
    if (copy == NULL) {
        return NULL;
    }

    cursor = copy;
    while ((found = strstr(cursor, sep)) != NULL) {
        char *segment;
        size_t len = (size_t)(found - cursor);
        if (count == 0) {
            items = (char **)malloc(sizeof(char *) * 4);
            if (items == NULL) {
                free(copy);
                return NULL;
            }
        } else {
            char **grown = (char **)realloc(items, sizeof(char *) * (count + 4));
            if (grown == NULL) {
                for (i = 0; i < count; ++i) {
                    free(items[i]);
                }
                free(items);
                free(copy);
                return NULL;
            }
            items = grown;
        }
        segment = (char *)malloc(len + 1);
        if (segment == NULL) {
            for (i = 0; i < count; ++i) {
                free(items[i]);
            }
            free(items);
            free(copy);
            return NULL;
        }
        memcpy(segment, cursor, len);
        segment[len] = '\0';
        items[count++] = segment;
        cursor = found + strlen(sep);
    }

    if (count == 0) {
        result = alsh_strdup(input);
        free(copy);
        return result;
    }

    if (count > 0 && *cursor != '\0') {
        size_t len = strlen(cursor);
        char *segment = (char *)malloc(len + 1);
        if (segment == NULL) {
            for (i = 0; i < count; ++i) {
                free(items[i]);
            }
            free(items);
            free(copy);
            return NULL;
        }
        memcpy(segment, cursor, len + 1);
        items[count++] = segment;
    }

    result = alsh_array_to_string(items, count);
    for (i = 0; i < count; ++i) {
        free(items[i]);
    }
    free(items);
    free(copy);
    return result;
}

char *alsh_std_lines(const char *input) {
    return alsh_std_split(input, "\n");
}

char *alsh_std_padleft(const char *input, int width, char fill) {
    size_t len;
    size_t pad;
    char *result;
    if (input == NULL) {
        input = "";
    }
    len = strlen(input);
    if ((size_t)width <= len) {
        return alsh_strdup(input);
    }
    pad = (size_t)width - len;
    result = (char *)malloc(len + pad + 1);
    if (result == NULL) {
        return NULL;
    }
    memset(result, fill, pad);
    memcpy(result + pad, input, len + 1);
    return result;
}

char *alsh_std_padright(const char *input, int width, char fill) {
    size_t len;
    size_t pad;
    char *result;
    if (input == NULL) {
        input = "";
    }
    len = strlen(input);
    if ((size_t)width <= len) {
        return alsh_strdup(input);
    }
    pad = (size_t)width - len;
    result = (char *)malloc(len + pad + 1);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, input, len);
    memset(result + len, fill, pad);
    result[len + pad] = '\0';
    return result;
}

char *alsh_std_format_gb(double kb) {
    return alsh_format("%.3f", kb / 1000.0 / 1000.0 / 1000.0);
}

char *alsh_std_format_gib(double kb) {
    return alsh_format("%.3f", kb / 1024.0 / 1024.0 / 1024.0);
}

char *alsh_std_format_mb(double kb) {
    return alsh_format("%.3f", kb / 1000.0 / 1000.0);
}

char *alsh_std_format_mib(double kb) {
    return alsh_format("%.3f", kb / 1024.0 / 1024.0);
}

size_t alsh_std_sizeof(const char *value) {
    return value == NULL ? 0 : strlen(value);
}

size_t alsh_std_sizeoft(const char *value) {
    return alsh_std_sizeof(value);
}
