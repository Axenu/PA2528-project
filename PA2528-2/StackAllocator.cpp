#include "StackAllocator.h"

static const size_t SIZE_OF_ALLOC_OFFSET = sizeof(size_t);
static_assert(SIZE_OF_ALLOC_OFFSET == 4, "Allocation offset has wrong size.");

StackAllocator::StackAllocator(size_t sizeStack, size_t alignment)
{
	// default offset
	m_offset = 0;

	// store alignment
	m_alignment = alignment;

	// store size
	m_sizeStack = sizeStack;

	m_elements = 0;

	// allocate memory from the OS to the stack
	if (m_alignment > 0)
#if defined(__WIN32) || defined(WIN32)  || defined(_WIN32)
		m_start = _aligned_malloc(sizeStack * sizeof(size_t), alignment);
#else
		// m_start = aligned_alloc(alignment, sizeStack * sizeof(size_t));
		posix_memalign(&m_start, alignment, sizeStack * sizeof(size_t));
#endif
	else
		m_start = malloc(sizeStack * sizeof(size_t));
		
	// store a pointer to the end of the memory block
	m_end = static_cast<char*>(m_start) + sizeStack;

#ifdef TRACK_MEMORY
	MemoryTracker::addReserved(reinterpret_cast<void*>(m_start), sizeStack * sizeof(size_t), ID);
#endif

	std::cerr << "Stack start: " << m_start << " | Stack end:" << m_end << std::endl;
	std::cerr << "Size of stack: " << _msize(m_start) << std::endl;

	// set the current "end" of the used memory as the start pointer
	m_ptr_current = m_start;
}

StackAllocator::~StackAllocator()
{
	free(m_start); // clear the stack
}

size_t StackAllocator::getSizeOfMemory()
{
	return m_sizeStack * sizeof(size_t);
}

void* StackAllocator::alloc_internal(size_t size)
{
	//std::cerr << "Stack allocating at address: " << m_ptr_current <<" | Size: "<< size << std::endl;
	void* current_pointer; // points at the start of the block

	current_pointer = m_ptr_current;

	// move the head to the start of the next block
	//m_offset += size * sizeof(size_t);
	m_offset += size;
	m_ptr_current = static_cast<char*>(m_ptr_current) + size;
	size_t memRemaining = static_cast<char*>(m_end) - static_cast<char*>(m_ptr_current);
	//std::cerr << "Stack remaining memory: " << memRemaining << std::endl;
	//std::cerr << "Stack offset: " << m_offset << std::endl;
	//std::cerr << "Elements in stack: " << m_elements << std::endl;

	// check if out of memory
	if (m_ptr_current > m_end)
	{
		std::cerr << "Out of memory: " << m_ptr_current << " | " << m_end << std::endl;
		m_ptr_current = static_cast<char*>(m_ptr_current) - m_offset;
		m_offset -= size;
		return nullptr;
	}

	m_elements++;

	//m_offset+=4;
	return current_pointer;
}

void StackAllocator::dealloc_internal(void* p) // no in pointer needed?
{
	if (m_ptr_current > m_start) // make sure the stack pointer is pointing ahead of the start of the stack to prevent underflow errors
	{
		// move the stack pointer back to the previous block
		//m_ptr_current = static_cast<char*>(m_ptr_current) - m_offset * sizeof(size_t);
		//m_ptr_current = m_start;
		
		//m_ptr_current = p;
		//size_t blockSize = 12;
		//m_offset -= blockSize;
		//m_ptr_current = static_cast<char*>(m_ptr_current) - blockSize;
		m_elements--;
		m_ptr_current = p;
		m_offset = static_cast<char*>(m_ptr_current) - m_start;
		//std::cerr << "Stack deallocating at address: " << m_ptr_current << std::endl;
		//std::cerr << "Stack offset: " << m_offset << " | blockSize: " << blockSize << std::endl;

		if (m_ptr_current < m_start) // check for underflow
		{
			//std::cerr << "No block available to remove from stack, underflow prevented." << std::endl;
			m_ptr_current = m_start;
		}
	}

	//reset(); // placeholder work around
	//std::cerr << "Elements in stack: " << m_elements << std::endl;
}

void StackAllocator::reset()
{
	// reset the stack pointer back to the start of the stack
	m_ptr_current = m_start;
	m_elements = 0;
}
