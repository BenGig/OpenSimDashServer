#include "stdafx.h"

#define PUSHSLEEP 50  // milliseconds
const char * WS_CMD = "TELL";

static struct mg_serve_http_opts server_opts;

struct dataServed
{
	char * address = NULL;
	char * document_root = NULL;
	ConnectorScheduler * simreader = NULL;
};

static dataServed * data = NULL;
static LiveItemRegistry itemRegistry;
static std::mutex pusherIsRunning;
static std::mutex pusherShouldEnd;
static bool webserverRunning = false;						// to signal end of execution

/*
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

void DataPusher(void *pParam)
{
	mg_connection *nc = (mg_connection *)pParam;
	if (!pusherIsRunning.try_lock())
		return;

	while (!pusherShouldEnd.try_lock())
	{
		if (itemRegistry.items.size() > 0)
		{
			std::string * data = itemRegistry.ChangedItemsJSON();

			if (data != nullptr)
			{
				if (transmit(nc, data->c_str(), data->length()))
				{
					Sleep(PUSHSLEEP);
				}
				else
				{
					itemRegistry.Empty();
					delete data;
					pusherIsRunning.unlock();
					return;
				}
				delete data;
			} else {
				Sleep(PUSHSLEEP);
			}			
		}
	}
	
}
*/

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
	char buf[10024] = { 0 };
	int msg[4];
	WSRequest * wsReq;
//	char ws_command[5]; char ws_arg[3];

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::string response;

	struct http_message *hm = (struct http_message *) p;
	struct websocket_message *wm = (struct websocket_message *) p;

	switch (ev) {
	case MG_EV_HTTP_REQUEST:

		memcpy(buf, hm->uri.p, hm->uri.len);
		if (strstr(buf, "event.json"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
			switch (webserverSim) {
			case SIM_RF:
				ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
				std::wstring * str = conn->sd->EventJson(conn->SimName());
				response = cv.to_bytes(*str);
				mg_printf_http_chunk(nc, response.c_str());
				delete str;
			}
			mg_send_http_chunk(nc, "", 0);
		}
		else if (strstr(buf, "scoring.json"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
			switch (webserverSim) {
			case SIM_RF:
				ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
				std::wstring * str = conn->sd->ScoringJson();
				response = cv.to_bytes(*str);
				mg_printf_http_chunk(nc, response.c_str());
				delete str;
			}
			mg_send_http_chunk(nc, "", 0);
		}
		else if (strstr(buf, "dictionary.json"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
			switch (webserverSim) {
			case SIM_RF:
				ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
				std::wstring * str = conn->sd->elementRegistry->JsonElements();
				response = cv.to_bytes(*str);
				mg_printf_http_chunk(nc, response.c_str());
				delete str;
			}
			mg_send_http_chunk(nc, "", 0);
		}

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

/*
		memcpy(ws_command, msg, 4);
		ws_command[4] = '\0';
		printf("Comparing command '%s' with '%s'\n", ws_command, WS_CMD);

		// new consumer
		if (strcmp(ws_command, WS_CMD) == 0)
		{
			printf("Pusher starting? %s\n", ws_command);
			memcpy(ws_arg, msg + strlen(ws_command), 3);
			LiveItem * liveItem = new LiveItem();
			int itemId;
			try
			{
				itemId = std::stoi(ws_arg);
			}
			catch (const std::invalid_argument&)
			{
				printf("Not a number: %s\n", ws_arg);
				break;
			}
			liveItem->RegisterFor(itemId, &itemRegistry);
			printf("Registered for %d\n", itemId);


			if (pusherIsRunning.try_lock())
			{

				pusherIsRunning.unlock();
				_beginthread(DataPusher, 0, nc);
			}
		}

		printf("Got message: %s.\n", msg);
		break;
*/
	case MG_EV_CLOSE:
		printf("Connection ended.\n");
	default:
		break;
	}
}

// Main webserver thread (not worker thread!)
void server(void *pParam)
{
	data = (dataServed*)pParam;
	pusherShouldEnd.lock();

	ConnectorRF* crf = NULL;
	// ConnectorAC* cac = NULL;

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
				data->simreader->connector->Disconnect(); data->simreader->connector = NULL;
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
				_tprintf(TEXT("Connected to %s.\n"), data->simreader->connector->SimName()->c_str());
				goto connected;
			}
			else
			{
				webserverSim = 0;
				delete data->simreader; data->simreader = NULL;
				Sleep(1000);
			}
			
			// TODO try to connect to Assetto Corsa

		}

	connected:
		mg_mgr_poll(&mgr, 10);
	}
	webserverRunning = false;
}

void launchServer(char * address, char * document_root)
{
	data = new dataServed;
	data->address = address;
	data->document_root = document_root;

	webserverRunning = true;

	_beginthread(server, 0, data);

}

void stopServer() {
	webserverRunning = false;
}

std::wstring * webserverSimName()
{
	if (data != NULL && data->simreader != NULL && data->simreader->connector != NULL && data->simreader->connector->SimName() != NULL)
	{
		return data->simreader->connector->SimName();
	}
	else
		return NULL;
}