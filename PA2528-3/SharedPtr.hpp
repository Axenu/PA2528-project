#ifndef __SHARED_PTR__
#define __SHARED_PTR__

#include "Atomic.hpp"
#include <cstring>

template<typename T>
class SharedPtr {
    public:
        SharedPtr(T* ptr = nullptr);
        SharedPtr(const SharedPtr<T>& other);
        ~SharedPtr();

        SharedPtr& operator=(const SharedPtr<T>& other);
        SharedPtr& operator=(T* other);

        T& operator*() const;
        T* operator->() const;
        bool operator==(const SharedPtr<T>& other) const;
        bool operator!=(const SharedPtr<T>& other) const;
        operator bool() const;


        size_t getReferenceCount() const;
        T* get() const;


    private:
        T* swapPtr(T* ptr);
        T* assign(const SharedPtr<T>& other);
        T* assign(T* other);
        void destroy();

    private:
        T* mPtr = nullptr;
        Atomic* mReferenceCount;
};

#include "SharedPtr.inl"

#endif // __SHARED_PTR__



