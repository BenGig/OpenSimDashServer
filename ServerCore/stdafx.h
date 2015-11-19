// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include <string>
#include <ctime>
#include <chrono>
#include <ctype.h>
#include <iostream>
#include <codecvt>
#include <process.h>
#include <tchar.h>
#include <winsock.h>


// TODO: reference additional headers your program requires here
#include "../DataTransfer.hpp"

#include "TelemetryData.hpp"
#include "SimDataElements.hpp"
#include "SimDataElementsManager.hpp"
#include "SimDataElementsManagerVar.hpp"
#include "SimData.hpp"
#include "Connector.hpp"
#include "ConnectorRF.hpp"
#include "RetrieverThread.hpp"

#include "WebServer.hpp"
