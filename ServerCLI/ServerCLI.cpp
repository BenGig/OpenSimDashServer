// ServerCLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char * address[64];

int main(int argc, char *argv[])
{
	// command line parsing
	TCLAP::CmdLine cmd("OpenSimDashServer", ' ', "0.1");
	TCLAP::ValueArg<std::string> portArg("p", "port", "Network port", false, "8080", "string");
	cmd.add(portArg);
	TCLAP::ValueArg<std::string> dashpathArg("d", "document_root", "Webserver starting point", false, "dashboards", "string");
	cmd.add(dashpathArg);

	cmd.parse(argc, argv);
	std::string path = dashpathArg.getValue();
	std::string port = portArg.getValue();

//	int j = getchar();
	launchServer(port, path);

	printf("Running on port %s, press CTRL-C to abort.\n", port.c_str());
	int i = getchar();
	return 0;
}

