#pragma once

#include "stdafx.h"

const int WS_MSG_INIT = 0;
const int WS_MSG_TELL = 1;
const int WS_MSG_NEWID = 2;

struct pusherEnv {
	std::mutex pusherIsRunning;
	std::mutex pusherStop;
	mg_connection * nc;
	LiveItemRegistry * itemRegistry;
};

class WSRequest
{
public:
	WSRequest() {};
	WSRequest(unsigned char msg[4], mg_connection * nc);

	unsigned int clientId;
	unsigned int command;
	unsigned int argument;
	unsigned int option;

private:

};

class WSSession
{
public:
	WSSession(unsigned char clientId, mg_connection * nc);
	~WSSession();

	unsigned char id;
private:
	LiveItemRegistry registry;
	mg_connection * connection;
	pusherEnv pData;

	friend class WSRequest;
};

