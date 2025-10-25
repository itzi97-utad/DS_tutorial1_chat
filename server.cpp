#include "clientManager.h"
#include "utils.h"
#include <csignal>
#include <iostream>
#include <thread>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void eventHandler(int s) {
	cout << endl << "Caught signal " << s << ", server closing" << endl;
	exit(1);
}

int main(int argc, char **argv) {

	signal(SIGINT, eventHandler);

	bool exit = false;

	// start/open the server in a free port : 1067
	cout << "Server opening port\n";
	int serverPortId = initServer(1067);
	cout << "Server port opened, waiting for connections\n";
	while (!exit) {
		// wait for connections
		while (!checkClient())
			usleep(100);
		// attend client
		int clientId = getLastClientID();
		cout << "Client " << clientId << " connected\n";
		// send/recv messages
		thread *th = new thread(clientManager::resolveClientMessages, clientId);
	}
	close(serverPortId);
	return 0;
}
