#ifndef __SPIN_LOCK__
#define __SPIN_LOCK__

#include "Atomic.hpp"

class SpinLock {
    public:
        SpinLock();

        void lock();
        void unlock();

    private:
        Atomic mIsLocked;
};

#endif // __SPIN_LOCK__

