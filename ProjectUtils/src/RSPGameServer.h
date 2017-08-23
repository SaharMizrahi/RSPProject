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
	bool isUserAlreadyLogin(char* username);

	string encryptDecrypt(string toEncrypt);
	/**
	 * This method authenticate user if cmd is login, and if cmd is register he will be write in the valid users list
	 */
	int authenticateClient(char* cmd,char* username,char* password);
	/**
	 * After new client is connect this method manage the authentication order
	 */
	bool handleConnectedClient(TCPSocket* sock);
	/**
	 * This method add new client to the users vector
	 */
	void addNewClient(TCPSocket* sock,char* username,int port);
public:
	 virtual void run();
	RSPGameServer(int port);
	virtual ~RSPGameServer();
};

} /* namespace networkingLab */

#endif /* SRC_RSPGAMESERVER_H_ */
