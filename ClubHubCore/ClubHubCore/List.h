#pragma once

template <typename T>
class List
{
	int count, size;
	T *data;

	void resize();
public:
	List();
	~List();

	void add( T data );
	T get( int index );
	T remove( int index );
	T& operator[] (const int x);
	int getCount();
	void clear();
};

#include "List.inl"