#include "stdafx.h"

#define PUSHSLEEP 50  // milliseconds
const char * WS_CMD = "TELL";

static struct mg_serve_http_opts server_opts;

struct dataServed
{
	char address[6];
	char document_root[MAX_PATH];
	ConnectorScheduler * simreader = NULL;
};
static char path[MAX_PATH];

static dataServed * data = NULL;
static LiveItemRegistry itemRegistry;
static std::mutex pusherIsRunning;
static std::mutex pusherShouldEnd;
static bool webserverRunning = false;						// to signal end of execution

/*
Event handler for any incoming requests.
*/
static void ev_handler(struct mg_connection *nc, int ev, void *p) {
	char buf[10024] = { 0 };
	unsigned char msg[4];
	WSRequest * wsReq;
//	char ws_command[5]; char ws_arg[3];

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::string response;

	struct http_message *hm = (struct http_message *) p;
	struct websocket_message *wm = (struct websocket_message *) p;

	switch (ev) {
	case MG_EV_HTTP_REQUEST:

		memcpy(buf, hm->uri.p, hm->uri.len);
		if (strstr(buf, "status.json"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nTransfer-Encoding: chunked\r\n\r\n");
			if (data->simreader == NULL)
				response.assign("{\"status\":false}");
			else
				response.assign("{\"status\":true}");
			mg_printf_http_chunk(nc, response.c_str());
			mg_send_http_chunk(nc, "", 0);
		}
		else if (strstr(buf, "event.json"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nTransfer-Encoding: chunked\r\n\r\n");
			Connector * conn = (Connector *)data->simreader->connector;
			std::wstring * str = conn->sd.eventJson(conn->simName());
			response = cv.to_bytes(*str);
			mg_printf_http_chunk(nc, response.c_str());
			delete str;
			mg_send_http_chunk(nc, "", 0);
		}
		else if (strstr(buf, "scoring.json"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nTransfer-Encoding: chunked\r\n\r\n");
			ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
			std::wstring * str = conn->sd.scoringJson();
			response = cv.to_bytes(*str);
			mg_printf_http_chunk(nc, response.c_str());
			delete str;
			mg_send_http_chunk(nc, "", 0);
		}
		else if (strstr(buf, "dictionary.json"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nTransfer-Encoding: chunked\r\n\r\n");
			ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
			std::wstring * str = conn->sd.elementRegistry->jsonElements();
			response = cv.to_bytes(*str);
			mg_printf_http_chunk(nc, response.c_str());
			delete str;
			mg_send_http_chunk(nc, "", 0);
		}

/*
		// TODO: remove the rest
		else if (strstr(buf, "json1"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
			switch (webserverSim) {
			case SIM_RF:
				ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
				std::wstring * str = SimRacingToolsJson1(conn->sd);
				response = cv.to_bytes(*str);
				mg_printf_http_chunk(nc, response.c_str());
				delete str;
			}
			mg_send_http_chunk(nc, "", 0);
		}
		else if (strstr(buf, "json2"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
			switch (webserverSim) {
			case SIM_RF:
				ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
				std::wstring * str = SimRacingToolsJson1(conn->sd);
				response = cv.to_bytes(*str);
				mg_printf_http_chunk(nc, response.c_str());
				delete str;
			}
			mg_send_http_chunk(nc, "", 0);
		}
		else if (strstr(buf, "json3"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
			switch (webserverSim) {
			case SIM_RF:
				ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
				std::wstring * str = SimRacingToolsJson3(conn->sd);
				response = cv.to_bytes(*str);
				delete str;
				mg_printf_http_chunk(nc, response.c_str());

			}
			mg_send_http_chunk(nc, "", 0);
		}
		*/

		else
		{
			mg_serve_http(nc, hm, server_opts); // serve file from disk
		}
		break;
	case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
		//printf("Got handshake.\n");

		break;
	case MG_EV_WEBSOCKET_FRAME:
		// Prepare and process request payload
		memcpy(msg, wm->data, wm->size);

		// Can be new client or registration for a data value
		wsReq = new WSRequest(msg, nc);
		delete wsReq;
		break;
	case MG_EV_CLOSE:
		break;
	default:
		break;
	}
}

// Main webserver thread (not worker thread!)
void server(void *pParam)
{
	data = (dataServed*)pParam;
	pusherShouldEnd.lock();

	// Prepare webserver
	struct mg_mgr mgr;
	struct mg_connection *nc;

	mg_mgr_init(&mgr, NULL);
	nc = mg_bind(&mgr, data->address, ev_handler);

	// Set up HTTP server parameters
	mg_set_protocol_http_websocket(nc);
	server_opts.document_root = data->document_root;  // Serve current directory
	server_opts.enable_directory_listing = "yes";


	// Search for simulation
	for (;;)
	{
		// should we quit?
		if (webserverRunning == false)
		{
			pusherShouldEnd.unlock();
			pusherIsRunning.lock();
			if (data->simreader != NULL)
			{
				data->simreader->connector->disconnect(); data->simreader->connector = NULL;
				webserverSim = 0;
			}
			// end webserver thread
			mg_mgr_free(&mgr);
			// give DataPusher time to relax
			Sleep(1000);
			pusherIsRunning.unlock();
			return;
		}

		// are we connected?
		if (data->simreader == NULL)
		{
			// try to connect to rFactor
			data->simreader = new ConnectorScheduler(SIM_RF);

			if (data->simreader->connector != NULL)
			{
				webserverSim = SIM_RF;
				_tprintf(TEXT("Connected to %s.\n"), data->simreader->connector->simName()->c_str());
				goto connected;
			}
			else
			{
				webserverSim = 0;
				delete data->simreader; data->simreader = NULL;
				Sleep(1000);
			}
			
			// try to connect to Assetto Corsa
			data->simreader = new ConnectorScheduler(SIM_AC);
			if (data->simreader->connector != NULL)
			{
				webserverSim = SIM_AC;
				_tprintf(TEXT("Connected to %s.\n"), data->simreader->connector->simName()->c_str());
				goto connected;
			}
			else
			{
				webserverSim = 0;
				delete data->simreader; data->simreader = NULL;
				Sleep(1000);
			}
		}

	connected:
		mg_mgr_poll(&mgr, 10);
	}
	webserverRunning = false;
}

/*
Start server to serve a directory on a given network address [host:]port
*/
void launchServer(std::string address, std::string document_root)
{
	
	data = new dataServed;
	strcpy_s(data->address, address.c_str());

	if (document_root.length()>0)
	{
		strcpy_s(data->document_root, document_root.c_str());
	}
	else
	{
		if (SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path) != S_OK)
		{
			std::cout << "I could not retrieve the user's local appdata directory!\n";
		}
		else
		{
			strcat(path, "\\OpenSimDash\\dashboards");
			strcpy(data->document_root, path);
		}
	}
	
	webserverRunning = true;

	_beginthread(server, 0, data);

}

void stopServer() {
	webserverRunning = false;
}

std::wstring * webserverSimName()
{
	if (data != NULL && data->simreader != NULL && data->simreader->connector != NULL && data->simreader->connector->simName() != NULL)
	{
		return data->simreader->connector->simName();
	}
	else
		return NULL;
}