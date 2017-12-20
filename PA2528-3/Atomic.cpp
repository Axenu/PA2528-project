#include "Atomic.hpp"


#include <windows.h>


Atomic::Atomic(long int value) {
    mValue = (volatile long int*)(size_t(mData) + (sizeof(long int) - size_t(mData) % sizeof(long int)) % sizeof(long int));
    *mValue = value;
}

Atomic& Atomic::operator++() {
    InterlockedIncrement(mValue);
    return *this;
}

Atomic& Atomic::operator--() {
    InterlockedDecrement(mValue);
    return *this;
}


Atomic& Atomic::operator+=(int i) {
    InterlockedAdd(mValue, i);
    return *this;
}

Atomic& Atomic::operator-=(int i) {
    InterlockedAdd(mValue, -i);
    return *this;
}

Atomic& Atomic::operator=(long int value)  {
    *mValue = value;
    return *this;
}

Atomic& Atomic::operator=(const Atomic& other) {
    *mValue = *other.mValue;
    return *this;
}

bool Atomic::operator==(long int value) {
    return *mValue == value;
}
bool Atomic::operator==(const Atomic& other) {
    return *mValue == *other.mValue;
}

long int Atomic::load() {
    return *mValue;
}

long int Atomic::exchange(long int value) {
    return InterlockedExchange(mValue, value);
}

bool Atomic::compareExchange(long int exchange, long int expected) {
    return InterlockedCompareExchange(mValue, exchange, expected) == expected;
}
