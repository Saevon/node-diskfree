#include <stdio.h>
#include "disk.h"
#include <inttypes.h>
#include <limits>
#include <cfloat>

#include "os_detect.h"

#ifdef IS_WIN
    #define VALID_PATH "C:"
#endif
#ifdef IS_UNIX
    #define VALID_PATH "/"
#endif



void assertEqual(uint64_t value1, uint64_t value2, const char *message) {
    if (value1 == value2) {
        return;
    }

    printf("%s: %" PRIu64 " != %" PRIu64 "\n", message, value1, value2);
}

void assertTrue(bool result, const char *message) {
    if (result) {
        return;
    }

    printf("%s\n", message);
}

int main(void) {
    uint64_t free, available, total;
    uint64_t err;

    err = GetDiskSpace(VALID_PATH, &available, &total, &free);

    assertEqual(err, 0, "error reading 'c:'");
    assertTrue(total > 0, "total disk space is wrong");
    assertTrue(total >= free, "total disk space less than free space??");
    assertTrue(total >= available, "total disk space less than available space??");
    assertTrue(free >= available, "free disk space is less than available space??");


    err = GetDiskSpace("c", &available, &total, &free);

    assertTrue(isErrBadPath(err), "no error reading invalid path");

    printf("avail: %" PRIu64 "\n", available);
    printf("free : %" PRIu64 "\n", free);
    printf("total: %" PRIu64 "\n", total);

    printf("Test Complete\n");

    //uint64_t val = DBL_MAX;
    //uint64_t val = //0xFFFFFFFFFFFFF800;
    uint64_t val = 0xFFFFFFFFFFFFF802;
    double val2 = (double) val;
    printf("%" PRIu64 "\n", val);
    printf("%f\n", val2);

    return 0;
}
