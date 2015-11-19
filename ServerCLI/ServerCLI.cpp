// ServerCLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char * address[64];

int main(int argc, char *argv[])
{
	*address = "8080";
	if (argc > 1)
	{
		strcpy(*address, argv[1]);
	}

	launchServer(*address, "dashboards");

	printf("Running on port %s, press CTRL-C to abort.\n", *address);
	int i = getchar();

	return 0;
}

