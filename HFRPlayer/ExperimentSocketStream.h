#pragma once

#include "SocketConnection.h"

#include "SafeQueue.h"
#include <vector>

#include <thread>
#include <mutex>

namespace Networking
{
	// networking socket stream to communicate with MATLAB
	// allowing characters to be sent to Matlab, notifying Renderer upon the receipt of a new String
	// provides an async write queue around primitive network sockets
	// 
	// RECEIVE FORMAT: STRING (terminated by a #)
	// SEND FORMAT: single char 
	class ExperimentSocketStream
	{
	public:

		// construct around a primitive Socket
		ExperimentSocketStream(SocketConnection* socket);
		// dtor
		virtual ~ExperimentSocketStream();

		// write element to the socket. Thread safe
		inline void write(const char element)
		{
			m_writeQueue.enqueue(element);
			m_sendWait.notify_one();
		}

	protected:

		SocketConnection* m_socket;
		SafeQueue<char> m_writeQueue;

	private:

		// send and receive loops
		void sendLoop();
		void receiveLoop();
		
		// threading, mutexes, sync
		bool m_shouldClose;
		std::thread* m_receiveThread;
		std::thread* m_sendThread;
		std::mutex m_sendMutex;
		std::condition_variable m_sendWait;
	};
}

