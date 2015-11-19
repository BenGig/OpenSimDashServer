#pragma once

#include "mongoose.h"
#include "stdafx.h"


#define WEBSERVER_OK 0
#define WEBSERVER_ERROR_BAD_ADDRESS 1

static std::wstring * webserverSimName = NULL;	// for UI output
static int webserverSim = 0;								// SIM_RF, SIM_AC...
static bool webserverRunning = false;						// to signal end of execution

void launchServer(char * address, char * document_root);
