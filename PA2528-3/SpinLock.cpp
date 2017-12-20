#include "SpinLock.hpp"

SpinLock::SpinLock() : mIsLocked(false) {
}

void SpinLock::lock() {
    while(!mIsLocked.compareExchange(true, false));
//    while(true) {
//        bool expected = false;
//        if(mIsLocked.compare_exchange_weak(expected, true)) {
//            return;
//        }
//    }
}

void SpinLock::unlock() {
    mIsLocked = false;
}
