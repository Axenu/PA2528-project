#ifndef __SEMAPHORE__
#define __SEMAPHORE__

#include <cstring>

#include "SharedPtr.hpp"

class Semaphore {
    public:
        Semaphore(size_t count = 0, unsigned short maxCount = -1);
        ~Semaphore();

        void signal(size_t count = 1) const;
        bool wait(size_t milliseconds) const;
        void wait() const;

    private:
        SharedPtr<void*> mSemaphoreHandle;
};

#endif // __SEMAPHORE__

