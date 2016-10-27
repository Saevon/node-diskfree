#include <stdint.h>
#include "os_detect.h"

#ifdef IS_WIN
    #include <windows.h>
    #include <WinError.h>
#endif
#ifdef IS_UNIX
    #include <sys/statvfs.h>
    #include <errno.h>
#endif


uint64_t GetDiskSpace(const char *path, uint64_t *available, uint64_t *total, uint64_t *free) {
    uint64_t error;

    #ifdef IS_WIN
        bool success = GetDiskFreeSpaceEx(
            path,
            reinterpret_cast<PULARGE_INTEGER>(available),
            reinterpret_cast<PULARGE_INTEGER>(total),
            reinterpret_cast<PULARGE_INTEGER>(free)
        );
        if (success == 1) {
            return 0;
        }

        // Windows API errorcode
        error = GetLastError();
        return error;
    #endif


    #ifdef IS_UNIX
        struct statvfs stats;
        int retVal = statvfs(
            path,
            &stats
        );

        // the return is either 0 or -1
        if (retVal == -1) {
            error = errno;
            return error;
        }

        // Convert the statvfs format
        *free = stats.f_bfree * stats.f_frsize;
        *available = stats.f_bavail * stats.f_frsize;
        *total = stats.f_blocks * stats.f_frsize;

        return 0;
    #endif

    // No OS?
    return 1;
}


bool isErrBadPath(uint64_t error) {
    return (false
        #ifdef IS_UNIX
            // Path doesn't exist
            || error == ENOENT
            // Path has too many redirects
            || error == ELOOP
            // Path is too long
            || error == ENAMETOOLONG
            // Not a directory
            || error == ENOTDIR
        #endif
        #ifdef IS_WIN
            // Path doesn't exist
            || error == ERROR_PATH_NOT_FOUND
            // Drive doesn't exist
            || error == ERROR_INVALID_DRIVE
        #endif
    );

}

bool isErrDenied(uint64_t error) {
    return (false
        #ifdef IS_UNIX
            // Permission Denied
            || error == EACCES
        #endif
        #ifdef IS_WIN
            // Permission Denied
            || error == ERROR_ACCESS_DENIED
        #endif
    );
}

bool isErrIO(uint64_t error) {
    return (false
        #ifdef IS_UNIX
            // IO Error
            || error == EIO
        #endif
        #ifdef IS_WIN
            || error == ERROR_NOT_READY
        #endif
    );
}

