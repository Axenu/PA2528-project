#ifndef __ATOMIC__
#define __ATOMIC__

#include <cstdint>

class Atomic {
    public:
        Atomic(long int value = 0);

        Atomic& operator++();
        Atomic& operator--();

        Atomic& operator+=(int i);
        Atomic& operator-=(int i);

        Atomic& operator=(long int value);
        Atomic& operator=(const Atomic& other);

        bool operator==(long int value);
        bool operator==(const Atomic& other);

        long int exchange(long int value);
        bool compareExchange(long int exchange, long int expected);

        long int load();


    private:
        char mData[16];
        volatile long int* mValue;
};

#endif // __ATOMIC__

