#pragma once

#include "stdafx.h"

static int webserverSim = 0;								// SIM_RF, SIM_AC...

void launchServer(std::string address, std::string document_root);
void stopServer();
std::wstring * webserverSimName();
