#include "alsh_std_common.h"

char *alsh_std_push(const char *value, const char *item) {
    size_t len1 = value == NULL ? 0 : strlen(value);
    size_t len2 = item == NULL ? 0 : strlen(item);
    char *result = (char *)malloc(len1 + len2 + 2);
    if (result == NULL) {
        return NULL;
    }
    if (len1 > 0) {
        memcpy(result, value, len1);
        result[len1] = '\n';
        memcpy(result + len1 + 1, item == NULL ? "" : item, len2 + 1);
    } else {
        memcpy(result, item == NULL ? "" : item, len2 + 1);
    }
    return result;
}

char *alsh_std_pop(const char *value) {
    const char *tail;
    size_t len;
    char *result;
    if (value == NULL || *value == '\0') {
        return alsh_strdup("");
    }
    tail = strrchr(value, '\n');
    if (tail == NULL) {
        return alsh_strdup("");
    }
    len = (size_t)(tail - value);
    result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, value, len);
    result[len] = '\0';
    return result;
}

char *alsh_std_first(const char *value) {
    const char *next;
    size_t len;
    char *result;
    if (value == NULL || *value == '\0') {
        return alsh_strdup("");
    }
    next = strchr(value, '\n');
    len = next == NULL ? strlen(value) : (size_t)(next - value);
    result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, value, len);
    result[len] = '\0';
    return result;
}

char *alsh_std_last(const char *value) {
    const char *tail;
    if (value == NULL || *value == '\0') {
        return alsh_strdup("");
    }
    tail = strrchr(value, '\n');
    if (tail == NULL) {
        return alsh_strdup(value);
    }
    return alsh_strdup(tail + 1);
}

char *alsh_std_slice(const char *value, size_t from, size_t to) {
    size_t len;
    size_t count;
    char *result;
    char **items = NULL;
    size_t item_count = 0;

    if (value == NULL || *value == '\0') {
        return alsh_strdup("");
    }
    items = alsh_parse_array(value, &item_count);
    if (items == NULL) {
        return alsh_strdup("");
    }
    if (from >= item_count) {
        alsh_free_array(items, item_count);
        return alsh_strdup("");
    }
    if (to > item_count) {
        to = item_count;
    }
    count = to - from;
    if (count == 0) {
        alsh_free_array(items, item_count);
        return alsh_strdup("");
    }
    len = 1;
    for (size_t i = from; i < to; ++i) {
        len += strlen(items[i]) + 1;
    }
    result = (char *)malloc(len);
    if (result == NULL) {
        alsh_free_array(items, item_count);
        return NULL;
    }
    result[0] = '\0';
    for (size_t i = from; i < to; ++i) {
        strcat(result, items[i]);
        if (i + 1 < to) {
            strcat(result, "\n");
        }
    }
    alsh_free_array(items, item_count);
    return result;
}

int alsh_std_containsa(const char *value, const char *item) {
    char **items = NULL;
    size_t count = 0;
    size_t i;
    int found = 0;
    if (value == NULL || item == NULL) {
        return 0;
    }
    items = alsh_parse_array(value, &count);
    if (items == NULL) {
        return 0;
    }
    for (i = 0; i < count; ++i) {
        if (strcmp(items[i], item) == 0) {
            found = 1;
            break;
        }
    }
    alsh_free_array(items, count);
    return found;
}

char *alsh_std_reverse(const char *value) {
    char **items = NULL;
    size_t count = 0;
    char *result;
    if (value == NULL || *value == '\0') {
        return alsh_strdup("");
    }
    items = alsh_parse_array(value, &count);
    if (items == NULL) {
        return alsh_strdup("");
    }
    result = (char *)malloc(1);
    if (result == NULL) {
        alsh_free_array(items, count);
        return NULL;
    }
    result[0] = '\0';
    for (size_t i = count; i > 0; --i) {
        char *combined;
        size_t len = strlen(result);
        size_t item_len = strlen(items[i - 1]);
        combined = (char *)realloc(result, len + item_len + 2);
        if (combined == NULL) {
            free(result);
            alsh_free_array(items, count);
            return NULL;
        }
        result = combined;
        if (len > 0) {
            result[len] = '\n';
            result[len + 1] = '\0';
            strcat(result, items[i - 1]);
        } else {
            strcpy(result, items[i - 1]);
        }
    }
    alsh_free_array(items, count);
    return result;
}

char *alsh_std_join(const char *value, const char *sep) {
    char **items = NULL;
    size_t count = 0;
    size_t len = 1;
    char *result;
    if (value == NULL || *value == '\0') {
        return alsh_strdup("");
    }
    items = alsh_parse_array(value, &count);
    if (items == NULL) {
        return alsh_strdup("");
    }
    if (sep == NULL) {
        sep = "";
    }
    for (size_t i = 0; i < count; ++i) {
        len += strlen(items[i]) + strlen(sep);
    }
    result = (char *)malloc(len);
    if (result == NULL) {
        alsh_free_array(items, count);
        return NULL;
    }
    result[0] = '\0';
    for (size_t i = 0; i < count; ++i) {
        if (i > 0) {
            strcat(result, sep);
        }
        strcat(result, items[i]);
    }
    alsh_free_array(items, count);
    return result;
}
