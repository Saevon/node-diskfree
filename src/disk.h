#include <stdint.h>


/**
 * Gets the amount of space available
 */
uint64_t GetDiskSpace(const char *path, uint64_t *available, uint64_t *total, uint64_t *free);


/**
 * format(type) that all the following error checking functions follow
 */
typedef bool (*diskErrFunc_t)(uint64_t);


/**
 * Returns whether the error is likely a Pathing error
 */
bool isErrBadPath(uint64_t error);


/**
 * Returns whether the error is likely a permissions error
 */
bool isErrDenied(uint64_t error);


/**
 * Returns whether the error is likely an IO error
 */
bool isErrIO(uint64_t error);
