#pragma once

#include "stdafx.h"


#define WEBSERVER_OK 0
#define WEBSERVER_ERROR_BAD_ADDRESS 1

static int webserverSim = 0;								// SIM_RF, SIM_AC...

void launchServer(char * address, char * document_root);
void stopServer();
std::wstring * webserverSimName();
