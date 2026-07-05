#include <stddef.h>
#include <string.h>

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
