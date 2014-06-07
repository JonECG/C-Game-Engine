#include "MemoryAllocator.h"
#include <iostream>
#include <assert.h>

#define TEST_MACRO( a ) MemoryAllocator::initialize(); a(); MemoryAllocator::shutdown();

struct Vector{ int x, y, z; };

void test1()
{
	//Primitive Allocations
	int *p1 = new int;
	*p1 = 1;
	
	int *p2 = new int;
	*p2 = 2;

	int *p3 = new int;
	*p3 = 3;

	assert( *p1 + *p2 + *p3 == 6 );
}

void test2()
{
	//Arrays, structs, and primitives less than 4 bytes
	int* p1 = new int[3];
	for( int i = 0; i < 3; i++ )
	{
		p1[i] = i;
	}

	Vector* p2 = new Vector();
	p2->x = 11;
	p2->y = 12;
	p2->z = 13;

	char* p3 = new char();
	*p3 = 14;

	assert( p1[2] + p2->y + *p3 == 28 );
}

void test3()
{
	//Out of memory test (Should break program when ran)
	for(;;)
	{
		int* p = new int;
		*p = -1;
	}
}

void test4()
{
	//Delete test
	int *p1 = new int;
	*p1 = 1;
	
	int *p2 = new int;
	*p2 = 2;

	delete p2;

	int *p3 = new int;
	*p3 = 3;

	assert( p2 == p3 );
}

void test5()
{
	//Fragmenting memory and traversing linked lists to find a suitable memblock
	double *p1 = new double;
	*p1 = 1;
	int *p2 = new int;
	*p2 = 2;
	double *p3 = new double;
	*p3 = 3;
	double *p4 = new double;
	*p4 = 4;
	double *p5 = new double;
	*p5 = 5;

	delete p4;
	delete p2;

	int *p6 = new int[2];
	*p6 = 6;

	assert( p4 == reinterpret_cast<double*>(p6) );
}

void test6()
{
	//Consolodating to full
	double *p1 = new double;
	*p1 = 1;
	double *p2 = new double;
	*p2 = 2;
	double *p3 = new double;
	*p3 = 3;
	double *p4 = new double;
	*p4 = 4;

	delete p1;
	delete p2;
	delete p3;
	delete p4;

	int n = MemoryAllocator::getNumberOfBlocks();
	assert( n == 1 );
}

void test7()
{
	//Fragmenting + consolodating + reallocation
	int *p1 = new int;
	*p1 = 1;
	int *p2 = new int;
	*p2 = 2;

	double *p3 = new double;
	*p3 = 3;

	delete p1;
	delete p2;

	double *p4 = new double;
	*p4 = 4;

	
	delete p3;
	delete p4;

	assert( p1 == reinterpret_cast<int*>(p4) );
}

void main()
{
	TEST_MACRO( test1 );
	TEST_MACRO( test2 );
	//TEST_MACRO( test3 ); //Testing out of memory will break program
	TEST_MACRO( test4 );
	TEST_MACRO( test5 );
	TEST_MACRO( test6 );
	TEST_MACRO( test7 );
}