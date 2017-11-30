#pragma once

#include "ExperimentSocketStream.h"

namespace Networking
{
	// factory/manager class for experiment sockets
	// responsible for the creation (open server port) and release of ports
	class ExperimentSocketManager
	{
	public:

		// initialise listening server with the provided port
		static void initialise(int port)
		{
			SocketConnection::staticInit();
			s_serverSocket = new SocketConnection(port);
			s_acceptThread = new std::thread(&ExperimentSocketManager::acceptLoop);
		}

		// clean up all opened ports (including server listener)
		static void cleanup()
		{
			delete s_serverSocket;
			s_acceptThread->join();
			for (auto socket : s_activeSockets)
			{
				delete socket;
			}
			s_activeSockets.clear();
			SocketConnection::staticCleanup();
		}

	private:
		// list of all active sockets
		static std::vector<ExperimentSocketStream*> s_activeSockets;

		// server socket
		static SocketConnection* s_serverSocket;

		// thread and loop for accepting new connections. These are immediately wrapped by an ExperimentSocketStream object
		static std::thread* s_acceptThread;
		static void acceptLoop();
	};
}