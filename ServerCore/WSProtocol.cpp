#include "stdafx.h"

#define PUSHSLEEP 50  // push interval, milliseconds

static std::map<int, WSSession*> sessions;
static std::map<int, WSSession*>::iterator it;

static bool transmit(struct mg_connection *nc, const char *msg, size_t len) {
	char buf[500];

	if (nc->err == 0)
	{
		snprintf(buf, sizeof(buf), "%p %.*s", nc, (int)len, msg);
		mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT, buf, strlen(buf));

		return true;
	}
	else
		return false;
}

// push data to websocket. Only data which has changed will be sent.
void DataPusher(void *pParam)
{
	pusherEnv * pData = (pusherEnv *)pParam;
	mg_connection *nc = pData->nc;
	if (!pData->pusherIsRunning.try_lock())
		return; // already running

	// if stop mutex is locked, then end thread
	while (!pData->pusherStop.try_lock())
	{
		if (pData->itemRegistry->items.size() > 0)
		{
			std::string * data = pData->itemRegistry->changedItemsJSON();

			if (data != nullptr)
			{
				if (!transmit(nc, data->c_str(), data->length()))
				{
					// transmission failed, stop thread
					delete data;
					pData->pusherIsRunning.unlock();
					return;
				}
				delete data;
			}
		}
		Sleep(PUSHSLEEP);
	}
}


WSRequest::WSRequest(unsigned char msg[4], mg_connection * nc)
{
	WSSession * session;
	LiveItem * liveItem;
	char initmsg[15];

	clientId = msg[0];
	command = msg[1];
	argument = msg[2];
	option = msg[3];

	switch (command)
	{
	case WS_MSG_INIT:
		// Search for 
		it = sessions.find(clientId);
		if (it == sessions.end())
		{
			// new client
			unsigned char id = rand() % 256;
			session = new WSSession(id, nc);
			sessions[id] = session;
			// tell client its connection id
			sprintf_s(initmsg, "id%d", id);
			transmit(nc, initmsg, strlen(initmsg));
		}
		break;
	case WS_MSG_TELL:
		session = sessions.at(clientId);
		
		liveItem = new LiveItem();
		liveItem->registerFor(argument, &session->registry);

		break;
	default:
		break;
	}


};


WSSession::WSSession(unsigned char clientId, mg_connection * nc)
{
	connection = nc;
	id = clientId;

	pData.itemRegistry = &registry;
	pData.nc = nc;

	pData.pusherStop.lock(); // keep locked until thread should end
	_beginthread(DataPusher, 0, &pData);

}

WSSession::~WSSession()
{
	// free stopper lock, thread will stop
	pData.pusherStop.unlock();
	while (!pData.pusherIsRunning.try_lock())
		Sleep(100);
	
	registry.clear();
	sessions.erase(id);
}