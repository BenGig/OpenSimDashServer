#include "stdafx.h"

static struct mg_serve_http_opts server_opts;

struct dataServed
{
	char * address = NULL;
	char * document_root = NULL;
	ConnectorScheduler * simreader = NULL;
};
static dataServed * data = NULL;

static void broadcast(struct mg_connection *nc, const char *msg, size_t len) {
	struct mg_connection *c;
	char buf[500];

	snprintf(buf, sizeof(buf), "%p %.*s", nc, (int)len, msg);
	for (c = mg_next(nc->mgr, NULL); c != NULL; c = mg_next(nc->mgr, c)) {
		mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, buf, strlen(buf));
	}
}

static void transmit(struct mg_connection *nc, const char *msg, size_t len) {
	char buf[500];

	snprintf(buf, sizeof(buf), "%p %.*s", nc, (int)len, msg);
	mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT, buf, strlen(buf));
}

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
	char buf[10024] = { 0 };
	char msg[1024];
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
				response = cv.to_bytes(*conn->sd->EventJson());
				mg_printf_http_chunk(nc, response.c_str());
			}
			mg_send_http_chunk(nc, "", 0);
		}
		else if (strstr(buf, "scoring.json"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
			switch (webserverSim) {
			case SIM_RF:
				ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
				response = cv.to_bytes(*conn->sd->ScoringJson());
				mg_printf_http_chunk(nc, response.c_str());
			}
			mg_send_http_chunk(nc, "", 0);
		}
		else if (strstr(buf, "dictionary.json"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
			switch (webserverSim) {
			case SIM_RF:
				ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
				response = cv.to_bytes(*conn->sd->elementRegistry->JsonElements(new std::wstring));
				mg_printf_http_chunk(nc, response.c_str());
			}
			mg_send_http_chunk(nc, "", 0);
		}
		else if (strstr(buf, "json1"))
		{
			mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
			switch (webserverSim) {
			case SIM_RF:
				ConnectorRF * conn = (ConnectorRF *)data->simreader->connector;
				std::wstring * str = SimRacingToolsJson1(conn->sd);
				response = cv.to_bytes(*str);
				delete str;
				mg_printf_http_chunk(nc, response.c_str());
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
				delete str;
				mg_printf_http_chunk(nc, response.c_str());
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
		broadcast(nc, "joined", 6);
		printf("Got handshake.\n");

		break;
	case MG_EV_WEBSOCKET_FRAME:
		memcpy(msg, wm->data, wm->size);
		msg[wm->size] = '\0';

		printf("Got message: %s.\n", msg);
		transmit(nc, "sent:1", 6);
		break;
	case MG_EV_CLOSE:
		broadcast(nc, "left", 4); break;
		printf("Connection ended.\n");
	default:
		break;
	}
}

// Main webserver thread (not worker thread!)
void server(void *pParam)
{
	data = (dataServed*)pParam;

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
			if (data->simreader != NULL)
			{
				data->simreader->connector->Disconnect(); data->simreader->connector = NULL;
				delete webserverSimName; webserverSimName = NULL;
				webserverSim = 0;
			}
			// end webserver thread
			mg_mgr_free(&mgr);
			return;
		}

		// are we connected?
		if (data->simreader == NULL)
		{
			// try to connect to rFactor
			data->simreader = new ConnectorScheduler(SIM_RF);

			if (data->simreader->connector != NULL)
			{
				webserverSimName = data->simreader->connector->SimName();
				webserverSim = SIM_RF;
				_tprintf(TEXT("Connected to %s.\n"), webserverSimName->c_str());
				goto connected;
			}
			else
			{
				delete webserverSimName;  webserverSimName = NULL;
				webserverSim = 0;
				delete data->simreader; data->simreader = NULL;
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
//	char debugdir[256] = "C:\\Users\\bgiger\\Documents\\Dokumente\\Projekte\\Entwicklung\\Games\\Dashboard\\webserver\\dashboards\\websocket\\";
//	data->document_root = debugdir;

	webserverRunning = true;

	_beginthread(server, 0, data);

}

