#include "stdafx.h"

static struct mg_serve_http_opts server_opts;

struct dataServed
{
	char * address = NULL;
	char * document_root = NULL;
	ConnectorScheduler * simreader = NULL;
};
static dataServed * data = NULL;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
	char buf[10024] = { 0 };
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
		else
		{
			mg_serve_http(nc, hm, server_opts); // serve file from disk
		}
		break;
	case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
		break;
	case MG_EV_WEBSOCKET_FRAME:
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
				webserverSimName = new std::wstring(data->simreader->connector->sim);
				webserverSim = SIM_RF;
				goto connected;
			}
			else
			{
				delete webserverSimName;  webserverSimName = NULL;
				webserverSim = 0;
				goto connected;
			}
			
			// TODO try to connect to Assetto Corsa

		}

	connected:
		mg_mgr_poll(&mgr, 1000);
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

