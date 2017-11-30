#include "ExperimentSocketManager.h"

using namespace Networking;

// initialsie static fields
SocketConnection* ExperimentSocketManager::s_serverSocket = nullptr;
std::vector<ExperimentSocketStream*> ExperimentSocketManager::s_activeSockets = {};
std::thread* ExperimentSocketManager::s_acceptThread = nullptr;

void ExperimentSocketManager::acceptLoop()
{
	while (true)
	{
		auto socket = s_serverSocket->accept();
		if (socket)
		{
			s_activeSockets.push_back(new ExperimentSocketStream(socket));
		}
		else
		{
			return;
		}
	}
}
