/*
 * ConnectedSocketsHandler.h
 *
 *  Created on: Aug 11, 2017
 *      Author: user
 */

#ifndef CONNECTEDSOCKETSHANDLER_H_
#define CONNECTEDSOCKETSHANDLER_H_
#include "MThread.h"
#include "RSPUser.h"
#include<iostream>
#include<stdio.h>
#include <vector>
#include "MultipleTCPSocketsListener.h"
#include "TCPSocket.h"
using namespace std;
namespace networkingLab {

class ConnectedSocketsHandler : public MThread {
private :
	bool stop;
	MultipleTCPSocketsListener* listener;
	vector<TCPSocket*> serverSockets;
	vector<RSPUser*> usersVector;
	//when client send msg this method handle it
	void handleClientMsg(TCPSocket* socket);
	//this method send HS to user
	void showHS(TCPSocket* socket);
	//this method show all connected users
	void showUsers(TCPSocket* socket);
	//this method start game with two users
	void startGame(TCPSocket* inviter,char* invitedUser);
	//this method set user as available for game
	void setUserAvailable(TCPSocket* socket);
	//this method set user as unavailable for game
	void setUserUnAvailable(TCPSocket* socket);
	//this method disconnect user
	void disconnectUser(TCPSocket* socket);
public:
	//this method add a new client to our server, and start listen also to his socket
	void addClient(char * username,TCPSocket* socket);
	//listening to all users sockets
	void run();
	ConnectedSocketsHandler();
	virtual ~ConnectedSocketsHandler();

};

} /* namespace networkingLab */

#endif /* CONNECTEDSOCKETSHANDLER_H_ */
