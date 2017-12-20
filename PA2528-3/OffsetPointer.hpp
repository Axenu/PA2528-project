#ifndef OFFSET_POINTER_HPP
#define OFFSET_POINTER_HPP

// Contains a pointer that is offset from the pointer returned from its original malloc() call.
// The memory is freed in the destructor, or when free() is called.
template<typename T>
class OffsetPointer {
public:
	OffsetPointer();
	OffsetPointer(T* ptr, unsigned int offset);

	~OffsetPointer();

	void freeOffsetMemory();

	T* getPointer();

private:
	T* pointer;
	unsigned int offset;
};


template<typename T>
inline OffsetPointer<T>::OffsetPointer()
{
	pointer = nullptr;
	offset = 0;
}

template<typename T>
inline OffsetPointer<T>::OffsetPointer(T * ptr, unsigned int offset)
{
	pointer = ptr;
	this->offset = offset;
}

template<typename T>
inline OffsetPointer<T>::~OffsetPointer()
{
	freeOffsetMemory();
}

template<typename T>
inline void OffsetPointer<T>::freeOffsetMemory()
{
	if (pointer)
		free(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pointer) - offset));
}

template<typename T>
inline T* OffsetPointer<T>::getPointer()
{
	return pointer;
}

#endif