#include "alsh_std_common.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
