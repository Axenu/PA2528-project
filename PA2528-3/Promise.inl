#include <cassert>

template<typename T>
Promise<T>::Promise(SharedPtr<SharedPtr<T>> promise, SharedPtr<bool> isReady, Semaphore semaphore)
: mPromise(promise)
, mIsReady(isReady)
, mSemaphore(semaphore) {
}

template<typename T>
bool Promise<T>::wait(size_t milliseconds) const {
    if(!*mIsReady) {
        bool ret = mSemaphore.wait(milliseconds);
        if(ret) {
            assert(*mIsReady);
        }
        return ret;
    }
    return true;
}


template<typename T>
void Promise<T>::wait() const {
    if(!*mIsReady) {
        mSemaphore.wait();
        assert(*mIsReady);
    }
}


template<typename T>
bool Promise<T>::isReady() const {
    return *mIsReady;
}


template<typename T>
T Promise<T>::get() const {
    wait();
    return **mPromise;
}

