#pragma once

class Message;

#include "definition.h"

template<typename T>
class List;

class ManagedSocket;

namespace std
{
	template<class _Fty>
	class function;
}

class EXPORT Session
{
	bool isHost, isActive;
	List<ManagedSocket*> *sockets;
	std::function<void()> **commandKeys;

	Session( bool isHost );
	//~Session(void);
	static void hosting( Session* session );
public:
	static Session* host();
	static Session* join( const char* ip );

	void receiveMessage( Message* message );
	void sendMessage( Message* message );
	void stop();
};

