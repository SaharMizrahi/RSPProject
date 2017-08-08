/*
 * Client.h
 *
 *  Created on: Aug 8, 2017
 *      Author: user
 */

#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_
#include<iostream>
#include<stdio.h>
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "User.h"
#include <list>
using namespace std;
namespace networkingLab {

class Client {
private:
	User* identity;
	TCPSocket* serverSocket;//sync
	TCPSocket* gameSocket;//sync
	UDPSocket* otherClientSocket;//Async
	bool connect();//connect to the server
	bool disconnect();//diconnect from server

public:
	void setGameSocket(string ip,int port);
	bool login(string username,string password);
	bool registerToServer(string username,string password);
	void startGame(string username);//if username is null, it will start a random
	void exit();
	list<string> showAvailableUsers();
	list<string> showHighscoors();
	Client();
	virtual ~Client();
};

} /* namespace networkingLab */

#endif /* SRC_CLIENT_H_ */
