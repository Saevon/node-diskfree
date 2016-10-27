#ifdef _WIN32
    #define IS_WIN32
    #define IS_WIN

    #ifdef _WIN64
        #undef IS_WIN32
        #define IS_WIN64
    #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
         // iOS Simulator
        #define IS_UNIX
    #elif TARGET_OS_IPHONE
        // iOS device
        #define IS_UNIX
    #elif TARGET_OS_MAC
        // Mac
        #define IS_UNIX
        #define IS_MAC
    #else
        #error "Unknown Apple platform"
    #endif
#elif __linux__
    // linux
    #define IS_UNIX
#elif __unix__ // all unices not caught above
    // Unix
    #define IS_UNIX
#elif defined(_POSIX_VERSION)
    // POSIX
    #define IS_UNIX
#else
    #error "Unknown compiler"
#endif
