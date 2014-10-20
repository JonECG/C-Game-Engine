#pragma once

#include "definition.h"

class EXPORT Message
{
	char *data;
	int dataSize;
	int internalDataSize;
	int readPointer, writePointer;

	void setSize( int size );
public:
	Message();
	Message( char *data, int dataSize );
	~Message();

	//void setData( char *data, int dataSize );
	char* getTotalData() const;
	char* getData() const;
	int getTotalSize() const;
	int getSize() const;

	void addInt( int addition );
	void addByte( char addition );

	int readInt();
	char readByte();
};

