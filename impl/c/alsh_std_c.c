#include <dirent.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static char *alsh_strdup(const char *value) {
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

static char *alsh_vformat(const char *fmt, va_list args) {
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

static char *alsh_format(const char *fmt, ...) {
    va_list args;
    char *value;
    va_start(args, fmt);
    value = alsh_vformat(fmt, args);
    va_end(args);
    return value;
}

static char *alsh_array_to_string(char **items, size_t count) {
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

static char **alsh_parse_array(const char *value, size_t *count) {
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

static void alsh_free_array(char **items, size_t count) {
    size_t i;
    if (items == NULL) {
        return;
    }
    for (i = 0; i < count; ++i) {
        free(items[i]);
    }
    free(items);
}

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

char *alsh_std_basename(const char *path) {
    const char *slash;
    if (path == NULL || *path == '\0') {
        return alsh_strdup("");
    }
    slash = strrchr(path, '/');
    if (slash == NULL) {
        return alsh_strdup(path);
    }
    return alsh_strdup(slash + 1);
}

char *alsh_std_dirname(const char *path) {
    const char *slash;
    size_t len;
    char *result;
    if (path == NULL || *path == '\0') {
        return alsh_strdup(".");
    }
    slash = strrchr(path, '/');
    if (slash == NULL) {
        return alsh_strdup(".");
    }
    if (slash == path) {
        return alsh_strdup("/");
    }
    len = (size_t)(slash - path);
    result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, path, len);
    result[len] = '\0';
    return result;
}

char *alsh_std_joinpath(const char *left, const char *right) {
    size_t left_len;
    size_t right_len;
    char *result;
    int needs_slash;
    if (left == NULL || *left == '\0') {
        return alsh_strdup(right == NULL ? "" : right);
    }
    if (right == NULL || *right == '\0') {
        return alsh_strdup(left);
    }
    left_len = strlen(left);
    right_len = strlen(right);
    needs_slash = (left[left_len - 1] != '/');
    result = (char *)malloc(left_len + right_len + needs_slash + 1);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, left, left_len);
    if (needs_slash) {
        result[left_len] = '/';
        memcpy(result + left_len + 1, right, right_len + 1);
    } else {
        memcpy(result + left_len, right, right_len + 1);
    }
    return result;
}

int alsh_std_isfile(const char *path) {
    struct stat st;
    if (path == NULL || stat(path, &st) != 0) {
        return 0;
    }
    return S_ISREG(st.st_mode) ? 1 : 0;
}

int alsh_std_isdir(const char *path) {
    struct stat st;
    if (path == NULL || stat(path, &st) != 0) {
        return 0;
    }
    return S_ISDIR(st.st_mode) ? 1 : 0;
}

int alsh_std_mkdir(const char *path) {
    if (path == NULL || *path == '\0') {
        return -1;
    }
    return mkdir(path, 0777) == 0 ? 0 : -1;
}

char *alsh_std_listdir(const char *path) {
    DIR *dir;
    struct dirent *entry;
    char *result = NULL;
    size_t total = 1;
    size_t count = 0;
    char *cursor;

    if (path == NULL || *path == '\0') {
        return alsh_strdup("");
    }

    dir = opendir(path);
    if (dir == NULL) {
        return alsh_strdup("");
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        total += strlen(entry->d_name) + 1;
        count++;
    }
    rewinddir(dir);

    result = (char *)malloc(total);
    if (result == NULL) {
        closedir(dir);
        return NULL;
    }
    cursor = result;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        size_t len = strlen(entry->d_name);
        memcpy(cursor, entry->d_name, len);
        cursor += len;
        *cursor++ = '\n';
    }
    closedir(dir);
    if (count > 0) {
        cursor[-1] = '\0';
    } else {
        result[0] = '\0';
    }
    return result;
}

char *alsh_std_which(const char *cmd) {
    char *path_env;
    char *cursor;
    char *saveptr;
    if (cmd == NULL || *cmd == '\0') {
        return alsh_strdup("");
    }
    if (strchr(cmd, '/') != NULL) {
        return access(cmd, X_OK) == 0 ? alsh_strdup(cmd) : alsh_strdup("");
    }
    path_env = getenv("PATH");
    if (path_env == NULL) {
        return alsh_strdup("");
    }
    path_env = alsh_strdup(path_env);
    if (path_env == NULL) {
        return NULL;
    }
    cursor = path_env;
    while ((saveptr = strchr(cursor, ':')) != NULL) {
        char *dir = cursor;
        size_t len;
        char *candidate;
        *saveptr = '\0';
        len = strlen(dir);
        candidate = (char *)malloc(len + strlen(cmd) + 2);
        if (candidate == NULL) {
            free(path_env);
            return NULL;
        }
        snprintf(candidate, len + strlen(cmd) + 2, "%s/%s", dir, cmd);
        if (access(candidate, X_OK) == 0) {
            free(path_env);
            return candidate;
        }
        free(candidate);
        cursor = saveptr + 1;
    }
    {
        char *dir = cursor;
        size_t len = strlen(dir);
        char *candidate = (char *)malloc(len + strlen(cmd) + 2);
        if (candidate == NULL) {
            free(path_env);
            return NULL;
        }
        snprintf(candidate, len + strlen(cmd) + 2, "%s/%s", dir, cmd);
        if (access(candidate, X_OK) == 0) {
            free(path_env);
            return candidate;
        }
        free(candidate);
    }
    free(path_env);
    return alsh_strdup("");
}

int alsh_std_pid(void) {
    return (int)getpid();
}

void alsh_std_sleep(int ms) {
    if (ms > 0) {
        usleep((useconds_t)ms * 1000);
    }
}

char *alsh_std_getuser(void) {
    const char *value = getenv("USER");
    if (value == NULL || *value == '\0') {
        value = getenv("LOGNAME");
    }
    return alsh_strdup(value == NULL ? "" : value);
}

double alsh_std_min(double a, double b) {
    return a < b ? a : b;
}

double alsh_std_max(double a, double b) {
    return a > b ? a : b;
}

double alsh_std_clamp(double value, double min, double max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

int alsh_std_even(long long n) {
    return (n % 2LL) == 0 ? 1 : 0;
}

int alsh_std_odd(long long n) {
    return (n % 2LL) != 0 ? 1 : 0;
}

size_t alsh_std_len(const char *value) {
    return value == NULL ? 0 : strlen(value);
}

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

void alsh_std_free_c_string(char *value) {
    free(value);
}
