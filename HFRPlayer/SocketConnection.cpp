#include "stdafx.h"
#include "SocketConnection.h"

#include <vector>
#include <iostream>

#include <WinSock2.h>
#include <Ws2tcpip.h>

#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <Windows.h>

#pragma comment( lib, "Ws2_32.lib" )  //_32 for both x64 and x86

using namespace Networking;

SocketConnection::SocketConnection(int port)
{
	memset(&m_address, 0, sizeof(m_address));
	m_address.sin_family = AF_INET;
	m_address.sin_addr.s_addr = INADDR_ANY;
	m_address.sin_port = htons(port);

	// create the socket
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET) {
		throw "failed to initialise socket";
	}

	// Bind the socket
	if (bind(m_socket, (LPSOCKADDR)&m_address, sizeof(m_address)) == SOCKET_ERROR)
	{
		throw "failed to bind server socket";
	}

	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw "failed to start listening";
	}
}

SocketConnection::SocketConnection(const std::string & address, int port)
{
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	char sPort[10];
	_itoa_s(port, sPort, 10, 10);

	// Resolve the server address and port
	if (getaddrinfo(address.c_str(), sPort, &hints, &result)) {
		throw "unabale to fetch server address";
	}

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET) {
		throw "failed to initialise socket";
	}

	for (ptr = result; ptr != 0; ptr = ptr->ai_next) {
		int iRes = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iRes == SOCKET_ERROR) {
			int nError = WSAGetLastError();
			if (nError != WSAEWOULDBLOCK) {
				continue; //we are doing asynchronous IO, so ignore this error
			}
		}

		freeaddrinfo(result);
		return;
	}

	freeaddrinfo(result);
	throw "failed to connect to a server";
}

SocketConnection::~SocketConnection()
{
	closesocket(m_socket);
}

void SocketConnection::staticInit()
{
	WSADATA WsaDat;
	int nResult = WSAStartup(MAKEWORD(2, 2), &WsaDat);
	if (nResult != 0)
	{
		throw "Cannot initialise Winsock";
	}
}

void SocketConnection::staticCleanup()
{
	WSACleanup();
}

SocketConnection::SocketConnection(SOCKET socket, sockaddr_in address) :
	m_address(address),
	m_socket(socket)
{
}

SocketConnection * SocketConnection::accept()
{
	int size = sizeof(sockaddr);
	sockaddr_in addrress;
	SOCKET socket = ::accept(m_socket, (struct sockaddr *)&addrress, &size);
	if (socket == INVALID_SOCKET)
	{
		std::cout << "server failed to accept new connection and returned with an error" <<std::endl;
		return nullptr;
	}

	return new SocketConnection(socket, addrress);
}

int SocketConnection::send(const char *buff, int size, int flags)
{
	return ::send(m_socket, buff, size, flags);
}

int SocketConnection::recv(char *buff, int size, int flags)
{
	int res = ::recv(m_socket, buff, size, flags);
	return res;
}

