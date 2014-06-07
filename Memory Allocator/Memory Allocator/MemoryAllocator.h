#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H

typedef char byte;

struct MemoryBlock
{
	size_t size;
	byte* nextBlock;
};

class MemoryAllocator
{
	static void* memory;
	static byte* head;
	static int numberOfBlocks;
	static const int SIZE = 96;

	inline static size_t getInstanceSize( void* address )
	{
		return *(reinterpret_cast<size_t*>( address ) - 1);
	}

	inline static size_t getMemSize( void* address )
	{
		return reinterpret_cast<MemoryBlock*>( address )->size;
	}

	inline static byte* getNextBlock( void* address )
	{
		return reinterpret_cast<MemoryBlock*>( address )->nextBlock;
	}

	inline static void setNextBlock( void* address, void* nextAddress )
	{
		reinterpret_cast<MemoryBlock*>( address )->nextBlock = reinterpret_cast<byte*>(nextAddress);
	}

	inline static void* pushPtr( void* ptr, int bytes )
	{
		return reinterpret_cast<void*>( reinterpret_cast<byte*>(ptr) + bytes );
	}

	//static MemoryAllocator singleton;
	static void sort();
	static byte* writeMemoryBlock( void* address, int size, byte* nextBlock );
	MemoryAllocator();
public:
	inline static int getNumberOfBlocks()
	{
		return numberOfBlocks;
	}

	static void initialize();
	static void shutdown();
	static void* alloNew( size_t size );
	static void alloDel( void* ptr );

};

void* operator new(size_t size);
void operator delete( void* ptr );

#endif