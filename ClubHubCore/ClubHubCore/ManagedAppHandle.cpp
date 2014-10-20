#include "ManagedAppHandle.h"

#include "Qt\qdebug.h"

void ManagedAppHandle::debug( const char* message )
{
	qDebug() << message;
}

void ManagedAppHandle::debug( double message )
{
	qDebug() << message;
}

void ManagedAppHandle::debug( long message )
{
	qDebug() << message;
}