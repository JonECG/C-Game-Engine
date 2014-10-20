#pragma once

class Session;
class Message;

#define _W64 __w64

#if defined(_WIN64)
    typedef unsigned __int64 UINT_PTR, *PUINT_PTR;
#else
    typedef _W64 unsigned int UINT_PTR, *PUINT_PTR;
#endif
typedef UINT_PTR SOCKET;

class ManagedSocket
{
	bool isActive;
	SOCKET socket;
	Session *session;
public:
	ManagedSocket(Session* session, SOCKET socket);
	//~ManagedSocket(void);

	void sendMessage( const Message* message );
	static void communicate( ManagedSocket* socket );
};

