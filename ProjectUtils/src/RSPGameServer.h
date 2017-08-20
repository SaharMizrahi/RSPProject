/*
 * RSPGameServer.h
 *
 *  Created on: Aug 17, 2017
 *      Author: user
 */

#ifndef SRC_RSPGAMESERVER_H_
#define SRC_RSPGAMESERVER_H_
#include<iostream>
#include<stdio.h>
#include "MThread.h"
#include "TCPSocket.h"
#include "vector"
#include "RSPUser.h"
#include "MultipleTCPSocketsListener.h"
#include "RSPClientHandler.h"
using namespace std;

namespace networkingLab {

class RSPGameServer : public MThread{
	vector<RSPUser*> usersVector;
	vector<TCPSocket*> socketsVector;

	TCPSocket* bindingSocket;
	RSPClientHandler* clientHandler;
	bool stopLogin;

	int authenticateClient(char* cmd,char* username,char* password);
	bool handleConnectedClient(TCPSocket* sock);
	void addNewClient(TCPSocket* sock,char* username,int port);
public:
	void run();
	RSPGameServer(int port);
	virtual ~RSPGameServer();
};

} /* namespace networkingLab */

#endif /* SRC_RSPGAMESERVER_H_ */
