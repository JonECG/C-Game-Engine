#include "Message.h"

Message::Message()
{
	internalDataSize = 255;
	data = new char[ internalDataSize ];
	readPointer = 0;
	writePointer = 0;

	data[0] = (char)0;

	setSize(0);
}
Message::Message( char *newData, int dataSize )
{
	internalDataSize = 255;
	data = new char[ internalDataSize ];
	readPointer = 0;
	writePointer = dataSize;

	for(int i = 0; i < dataSize; i++ )
	{
		data[i+1] = newData[i];
	}

	setSize(dataSize);
}
Message::~Message()
{
	delete data;
}

void Message::setSize( int size )
{
	data[0] = (char)size;
	dataSize = size;
}

//void setData( char *data, int dataSize );
char* Message::getTotalData() const
{
	return data;
}
char* Message::getData() const
{
	return data+1;
}
int Message::getTotalSize() const
{
	return dataSize + 1;
}
int Message::getSize() const
{
	return dataSize;
}

void Message::addInt( int addition )
{
	*reinterpret_cast<int*>(getData()+writePointer) = addition;
	writePointer += sizeof(int);
	setSize( getSize() + sizeof(int) );
}
void Message::addByte( char addition )
{
	getData()[writePointer] = addition;
	writePointer += sizeof(char);
	setSize( getSize() + sizeof(char) );
}

int Message::readInt()
{
	int result = *reinterpret_cast<int*>(getData()+readPointer);
	readPointer += sizeof(int);
	return result;
}
char Message::readByte()
{
	char result = getData()[readPointer];
	readPointer += sizeof(char);
	return result;
}