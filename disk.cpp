#include <windows.h>
#include <stdint.h>

uint64_t GetDiskSpace(const char *path, uint64_t *available, uint64_t *total, uint64_t *free) {
    BOOL success = GetDiskFreeSpaceEx(
        path,
        reinterpret_cast<PULARGE_INTEGER>(available),
        reinterpret_cast<PULARGE_INTEGER>(total),
        reinterpret_cast<PULARGE_INTEGER>(free)
    );
    if (success) {
        return 0;
    }

    uint64_t error = GetLastError();

    return error;
}
