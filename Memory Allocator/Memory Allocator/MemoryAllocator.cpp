#include "MemoryAllocator.h"
#include <malloc.h>
#include <algorithm>

void* MemoryAllocator::memory = nullptr;
byte* MemoryAllocator::head = nullptr;
int MemoryAllocator::numberOfBlocks = 0;

MemoryAllocator::MemoryAllocator()
{
}

void MemoryAllocator::initialize()
{
	memory = malloc( SIZE );
	head = reinterpret_cast<byte*>(memory);
	numberOfBlocks = 1;
	writeMemoryBlock( head, SIZE, nullptr );
}

void MemoryAllocator::shutdown()
{
	free( memory );
}

byte* MemoryAllocator::writeMemoryBlock( void* address, int size, byte* nextBlock )
{
	MemoryBlock* mem = reinterpret_cast<MemoryBlock*>(address);
	mem -> size = size;
	mem -> nextBlock = nextBlock;
	return reinterpret_cast<byte*>(address);
}

void* MemoryAllocator::alloNew( size_t size )
{
	//finalSize is used make sure allocated memory will be able to fit
	//a MemoryBlock when deleted
	size_t finalSize = size + sizeof(size_t);

	bool memExhausted = false;

	//If the size of the type is too small to fit a memBlock upgrade its size
	if ( finalSize < sizeof(MemoryBlock) )
	{
		finalSize = sizeof(MemoryBlock);
	}

	//Find a block that can fit this type
	byte* previousBlock = nullptr;
	byte* usingBlock = head;
	while ( getMemSize( usingBlock ) < finalSize )
	{
		previousBlock = usingBlock;
		usingBlock = getNextBlock( usingBlock );
	}

	//store copy of data on stack
	size_t prevSize = getMemSize( usingBlock );
	byte* prevMemory = getNextBlock( usingBlock );

	//If the remaining size cannot fit another memblock allocate the rest to this
	if ( prevSize - finalSize < sizeof(MemoryBlock) )
	{
		finalSize = prevSize;
		memExhausted = true;
	}

	//Allocate the memory and write its size before to be referenced when deleting 
	void* result = usingBlock+sizeof(size_t);
	*(reinterpret_cast<size_t*>(usingBlock)) = finalSize;

#ifdef _DEBUG
	//Fill in allocated memory with bc
	for( byte* b = reinterpret_cast<byte*>(result); b < (usingBlock + finalSize); b++ )
	{
		*b = 188;
	}
#endif

	if (memExhausted)
	{
		if( previousBlock != nullptr )
		{
			//Remove a segment and link the previous to the one after
			setNextBlock( previousBlock, prevMemory );
		}
		else
		{
			//Advance the head to the next block
			head = prevMemory;
		}
		numberOfBlocks--;
	}
	else
	{
		//Increment head and write the new memory block
		if( previousBlock == nullptr )
		{
			head += finalSize;
		}
		writeMemoryBlock( head, prevSize - finalSize, prevMemory );
	}

	return result;
}

void MemoryAllocator::alloDel( void* ptr )
{
	head = writeMemoryBlock( reinterpret_cast<size_t*>(ptr)-1, getInstanceSize( ptr ), head );
	numberOfBlocks++;

#ifdef _DEBUG
	//Fill in deallocated memory with de
	for( byte* b = reinterpret_cast<byte*>(ptr)+4; b < (reinterpret_cast<byte*>(ptr) + getInstanceSize( ptr ) - sizeof(size_t)); b++ )
	{
		*b = 222;
	}
#endif

	sort();
	//consolidate();
}

void MemoryAllocator::sort()
{
	//Find where each memory block is situated relative in memory
	MemoryBlock** pointers = reinterpret_cast<MemoryBlock**>(malloc(4*numberOfBlocks));

	MemoryBlock* dive = reinterpret_cast<MemoryBlock*>(head);
	for( int i = 0; i < numberOfBlocks; i++ )
	{
		pointers[i] = dive;
		dive = reinterpret_cast<MemoryBlock*>(dive->nextBlock);
	}

	std::sort( pointers, pointers + numberOfBlocks );

	//Figure out where they are situated on the linked list
	int* indices = reinterpret_cast<int*>(malloc(4*numberOfBlocks));

	dive = reinterpret_cast<MemoryBlock*>(head);
	for( int i = 0; i < numberOfBlocks; i++ )
	{
		for( int j = 0; j < numberOfBlocks; j++ )
		{
			if( pointers[j] == dive )
				indices[i] = j;
		}
		dive = reinterpret_cast<MemoryBlock*>(dive->nextBlock);
	}

	//Consolodate nearby blocks
	MemoryBlock* afterDive = reinterpret_cast<MemoryBlock*>(pointers[numberOfBlocks-1]);
	for( int i = numberOfBlocks-2; i >= 0; i-- )
	{
		dive = reinterpret_cast<MemoryBlock*>(pointers[i]);

		//Check if memory blocks are adjacent
		if ( pushPtr( dive, dive->size ) == afterDive )
		{
			//Update linked list before
			if( i > 1 )//indices[i-1] != 0 )
			{
				pointers[ indices[i-2] ] -> nextBlock = reinterpret_cast<byte*>(dive);
			}

			//Merge the two and pull nextblock
			//dive -> nextBlock = afterDive -> nextBlock;
			pointers[ indices[ i ] ] -> nextBlock = afterDive -> nextBlock;
			dive -> size += afterDive -> size;
			numberOfBlocks--;

#ifdef _DEBUG
			//Fill in merged memory with df
			for( byte* b = reinterpret_cast<byte*>(dive)+4+sizeof(size_t); b < (reinterpret_cast<byte*>(dive) + dive -> size - afterDive -> size ); b++ )
			{
				*b = 223;
			}
#endif

			if (head == reinterpret_cast<byte*>(afterDive))
				head = reinterpret_cast<byte*>(dive);
		}

		afterDive = dive;
	}

	free( pointers );
	free( indices );
}

void* operator new( size_t size )
{
	return MemoryAllocator::alloNew( size );
}

void operator delete( void* ptr )
{
	MemoryAllocator::alloDel( ptr );
}