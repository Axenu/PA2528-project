#ifndef __PROMISE__
#define __PROMISE__

#include "Semaphore.hpp"

template<typename T>
class Promise {
    public:
        Promise(SharedPtr<SharedPtr<T>> promise, SharedPtr<bool> isReady, Semaphore semaphore);

        bool wait(size_t milliseconds) const;
        void wait() const;

        bool isReady() const;
        T get() const;

    private:
        SharedPtr<SharedPtr<T>> mPromise;
        SharedPtr<bool> mIsReady;
        Semaphore mSemaphore;
};

#include "Promise.inl"

#endif // __PROMISE__

