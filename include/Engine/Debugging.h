#ifndef ENGINE_DEBUGGING_H
#define ENGINE_DEBUGGING_H

#define ENGINE_SUCCESS  0
#define ENGINE_ERROR    1
#define ENGINE_WARNING  2
#define ENGINE_LOADING  4

namespace Engine {
    namespace Debugging {
        void PrintSeparator();

        void PrintLog(const char* log, Uint8 type);
    }
}

#endif