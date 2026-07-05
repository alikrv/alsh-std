#include "alsh_std_common.h"

char *alsh_strdup(const char *value) {
    size_t len;
    char *copy;
    if (value == NULL) {
        value = "";
    }
    len = strlen(value) + 1;
    copy = (char *)malloc(len);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, value, len);
    return copy;
}

char *alsh_vformat(const char *fmt, va_list args) {
    va_list copy;
    int needed;
    char *buffer;

    va_copy(copy, args);
    needed = vsnprintf(NULL, 0, fmt, copy);
    va_end(copy);
    if (needed < 0) {
        return NULL;
    }

    buffer = (char *)malloc((size_t)needed + 1);
    if (buffer == NULL) {
        return NULL;
    }

    vsnprintf(buffer, (size_t)needed + 1, fmt, args);
    return buffer;
}

char *alsh_format(const char *fmt, ...) {
    va_list args;
    char *value;
    va_start(args, fmt);
    value = alsh_vformat(fmt, args);
    va_end(args);
    return value;
}

char *alsh_array_to_string(char **items, size_t count) {
    size_t total = 1;
    size_t i;
    char *result;
    char *cursor;

    if (items == NULL || count == 0) {
        return alsh_strdup("");
    }

    for (i = 0; i < count; ++i) {
        total += strlen(items[i] != NULL ? items[i] : "") + 1;
    }

    result = (char *)malloc(total);
    if (result == NULL) {
        return NULL;
    }
    cursor = result;
    for (i = 0; i < count; ++i) {
        const char *item = items[i] != NULL ? items[i] : "";
        size_t len = strlen(item);
        memcpy(cursor, item, len);
        cursor += len;
        *cursor++ = '\n';
    }
    cursor[-1] = '\0';
    return result;
}

char **alsh_parse_array(const char *value, size_t *count) {
    char *copy;
    char *cursor;
    char **items = NULL;
    size_t item_count = 0;
    size_t capacity = 0;

    if (count != NULL) {
        *count = 0;
    }
    if (value == NULL) {
        return NULL;
    }

    copy = alsh_strdup(value);
    if (copy == NULL) {
        return NULL;
    }

    cursor = copy;
    while (*cursor != '\0') {
        char *next = strchr(cursor, '\n');
        if (next != NULL) {
            *next = '\0';
        }
        if (item_count == capacity) {
            size_t new_capacity = capacity == 0 ? 4 : capacity * 2;
            char **grown = (char **)realloc(items, sizeof(char *) * new_capacity);
            if (grown == NULL) {
                free(copy);
                for (size_t i = 0; i < item_count; ++i) {
                    free(items[i]);
                }
                free(items);
                return NULL;
            }
            items = grown;
            capacity = new_capacity;
        }
        items[item_count++] = alsh_strdup(cursor);
        if (next == NULL) {
            break;
        }
        cursor = next + 1;
    }

    free(copy);
    if (count != NULL) {
        *count = item_count;
    }
    return items;
}

void alsh_free_array(char **items, size_t count) {
    size_t i;
    if (items == NULL) {
        return;
    }
    for (i = 0; i < count; ++i) {
        free(items[i]);
    }
    free(items);
}
