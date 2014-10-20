#include "ManagedSocket.h"

#include "Message.h"
#include "Session.h"
#include <thread>

#include <WinSock2.h>
#define SCK_VERSION2 0x0202

ManagedSocket::ManagedSocket(Session* session, SOCKET socket)
{
	this->session = session;
	this->socket = socket;
	isActive = true;
	std::thread t( communicate, this );
	t.detach();
}
//~ManagedSocket(void);

void ManagedSocket::sendMessage( const Message* message )
{
	int isSuccessful;
	isSuccessful = send( socket, message->getTotalData(), message->getTotalSize(), NULL );
}
void ManagedSocket::communicate( ManagedSocket* socket )
{
	while( socket->isActive )
	{
		long isSuccessful;
		char size;
		isSuccessful = recv( socket->socket, &size, 1, NULL );

		char *data = new char[ (int)size ];
		isSuccessful = recv( socket->socket, data, (int)size, NULL );

		Message *received = new Message( data, (int)size );
		socket->session->receiveMessage( received );
		delete data;
		delete received;
	}
}