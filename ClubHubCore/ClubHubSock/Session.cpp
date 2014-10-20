#include "Session.h"

#include <functional>
#include "List.h"
#include <thread>
//#include "ManagedSocket.h"

#include "Message.h"
#include "ManagedSocket.h"


//STD STUFF
#include <sdkddkver.h>
#include <conio.h>
#include <stdio.h>

//WINSOCK includes
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>

#define SCK_VERSION2 0x0202

Session::Session( bool isHost )
{
	this->isHost = isHost;
	isActive = false;
	sockets = new List<ManagedSocket*>;

	commandKeys = reinterpret_cast<std::function<void()>**>(malloc(sizeof(int*)*256*256));
}

void Session::hosting( Session* session )
{
	long isSuccessful;
	WSAData winSockData;
	WORD dllVersion;

	dllVersion = MAKEWORD( 2, 1 );
	isSuccessful = WSAStartup(  dllVersion, &winSockData );

	SOCKADDR_IN address;
	int addressSize = sizeof( address );

	SOCKET sockListen, sockConnection;

	sockConnection = socket( AF_INET, SOCK_STREAM, NULL );
	address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	address.sin_family = AF_INET;
	address.sin_port = htons(444);

	sockListen = socket(AF_INET, SOCK_STREAM, NULL );
	bind( sockListen, (SOCKADDR*)&address, sizeof(address ) );
	session->isActive = listen( sockListen, SOMAXCONN ) == 0;

	while( session->isActive )
	{
		std::cout << "Waiting For Connection" << std::endl;
		sockConnection = accept(sockListen, (SOCKADDR*)&address, &addressSize );
		if( sockConnection )
		{
			session->sockets->add( new ManagedSocket(session,sockConnection) );
			std::cout << "A connection was found" << std::endl;
			isSuccessful = send( sockConnection, "Hi.", 4, NULL );
		}
	}
}

Session* Session::host()
{
	Session *s = new Session(true);
	std::thread t( hosting, s );
	t.detach();
	return s;
}
Session* Session::join( const char* ip )
{
	long isSuccessful;
	WSAData winSockData;
	WORD dllVersion;

	dllVersion = MAKEWORD( 2, 1 );
	isSuccessful = WSAStartup(  dllVersion, &winSockData );

	Session *result = nullptr;// new Session(false);

	SOCKET sock;
	sock = socket( AF_INET, SOCK_STREAM, NULL );

	SOCKADDR_IN address;
	address.sin_addr.S_un.S_addr = inet_addr(ip);
	address.sin_family = AF_INET;
	address.sin_port = htons(444);

	if ( connect( sock, (SOCKADDR*)&address, sizeof(address) ) == 0 )
	{
		result = new Session(false);
		result -> isActive = true;
		result -> sockets -> add( new ManagedSocket(result,sock) );
	}

	
	return result;
}

void Session::sendMessage( Message* message )
{
	for( int i = 0; i < sockets->getCount(); i++ )
	{
		(*sockets)[i]->sendMessage( message );
	}
}
void Session::receiveMessage( Message* message )
{
	char *converter = new char[ message->getSize() + 1 ];
	converter[message->getSize()] = '\0';

	for( int i = 0; i < message->getSize(); i++ )
	{
		char current = message->getData()[i];
		converter[i] = (current == '\0') ? '|': current;
	}

	std::cout << "Message Received! Size: " << message->getSize() << "  Contents: " << converter << std::endl;

	delete converter;
}
void Session::stop()
{
	for( int i = 0; i < sockets->getCount(); i++ )
	{
		//(*sockets)[i]->
	}
	isActive = false;
}