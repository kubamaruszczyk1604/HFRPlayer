#include "ExperimentSocketStream.h"

#include "Renderer.h"

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
	char command = 0;
	std::stringstream stringBuff;
	
	int res = 1;
	while (!m_shouldClose)
	{
		while (res > 0)
		{
			res = m_socket->recv(&charBuff, 1, 0);

			if (res > 0)
			{
				if (!command)
				{
					command = charBuff;
				}
				else if (charBuff != '#')
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
		}

		if (res <= 0)
		{
			// error
			int error = WSAGetLastError();
			std::cout << "network receive error: "<<  error << std::endl;
			break;
		}
		else
		{
			// full string loaded
			std::cout << "received string from Matlab" << std::endl;

			// demultiplex based on command
			// F: framerate (int)
			// P: path (stirng)
			if (command == 'P')
			{
				Renderer::LoadTextures(stringBuff.str(), this);
			}
			else if (command == 'F')
			{
				int fps = 165;
				stringBuff >> fps;
				Renderer::SetFPS(fps);
				std::cout << "setting frame rate to " << fps << std::endl;
			}
			else
			{
				std::cout << "ignoring network packet, no command found." << std::endl;
			}
			
			stringBuff.str("");
			stringBuff.clear();
			charBuff = 0;
			command = 0;
		}
	}
}
