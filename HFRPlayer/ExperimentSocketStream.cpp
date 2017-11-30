#include "ExperimentSocketStream.h"

#include <sstream>
#include <string>
#include <iostream>

using namespace Networking;

ExperimentSocketStream::ExperimentSocketStream(SocketConnection * socket) :
	m_socket(socket),
	m_shouldClose(false)
{
	m_receiveThread = new std::thread(&ExperimentSocketStream::receiveLoop, this);
	m_sendThread = new std::thread(&ExperimentSocketStream::sendLoop, this);

	// issue welcome character
	this->write('W');
}

ExperimentSocketStream::~ExperimentSocketStream()
{
	m_shouldClose = true;
	m_sendWait.notify_one();
	m_receiveThread->detach();
	m_sendThread->join();
	delete m_receiveThread;
	delete m_sendThread;
	delete m_socket;
}

void ExperimentSocketStream::sendLoop()
{
	char data;
	while (!m_shouldClose)
	{
		// send all the waiting chars
		while (m_writeQueue.dequeue(data))
		{
			m_socket->send(&data, 1, 0);
		}

		// wait for write mutex to notify us
		std::unique_lock<std::mutex> lock(m_sendMutex);
		m_sendWait.wait(lock);
	}
}

void ExperimentSocketStream::receiveLoop()
{
	char charBuff = 0;
	std::ostringstream stringBuff;


	int res = 0;
	while (!m_shouldClose)
	{
		while (res >= 0)
		{
			res = m_socket->recv(&charBuff, 1, 0);
			if (charBuff != '#')
			{
				// new character
				stringBuff << charBuff;
			}
			else
			{
				// string terminated
				break;
			}
		}

		if (res < 0)
		{
			// error
			int error = WSAGetLastError();
			std::cout << "network receive error: "<<  error << std::endl;
			break;
		}
		else
		{
			// full string loaded
			std::cout << "received string: " << stringBuff.str() << std::endl;
			stringBuff.clear();
			charBuff = 0;
			// TODO: notify RENDERER
		}
	}
}
