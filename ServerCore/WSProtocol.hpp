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
	WSRequest(int msg[4], mg_connection * nc);

	int clientId;
	int command;
	int argument;
	int option;

private:

};

class WSSession
{
public:
	WSSession(int clientId, mg_connection * nc);
	~WSSession();

	int id;
private:
	LiveItemRegistry registry;
	mg_connection * connection;
	pusherEnv pData;

	friend class WSRequest;
};

