#pragma once

#include <WinSock2.h>

#include <thread>
#include <string>

namespace Networking
{
	// primitive network socket connection
	// the socket can either act as a tcp server or a tcp client
	class SocketConnection
	{
	public:
		// open a new server socket with the provided port number. Blocking
		SocketConnection(int port);

		// open a new client socket with the provided address and port number. Blocking
		SocketConnection(const std::string& address, int port);

		// delete socket
		~SocketConnection();

		// accept new connection on this server. Blocking
		SocketConnection* accept();

		// read data over this connection. Blocking
		int recv(char *buff, int size, int flags = 0);

		// send data over this connection. Blocking
		int send(const char *buff, int size, int flags = 0);

		static void staticInit();
		static void staticCleanup();

	protected:
		SOCKET m_socket;
		sockaddr_in m_address;

		// construct to wrap around a server-accepted socket
		SocketConnection(SOCKET socket, sockaddr_in address);
	};
}

