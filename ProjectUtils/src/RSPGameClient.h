/*
 * RSPGameClient.h
 *
 *  Created on: Aug 15, 2017
 *      Author: user
 */

#ifndef SRC_RSPGAMECLIENT_H_
#define SRC_RSPGAMECLIENT_H_
#include<iostream>
#include<stdio.h>
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "MThread.h"
using namespace std;
namespace networkingLab {

class RSPGameClient{
	TCPSocket* sock;
	char* username;
	char* password;
	char* cmd;

public:
	int recvFromOtherUser(UDPSocket* mySock);
	int policy(int c1,int c2);
	/**
	 * This method send to the server request to change user availability, if flag is true the user will return to msg from the server to start a RSP game
	 */
	void setUserAvailability(bool flag);
	/**
	 * This method manage RSP game order
	 */
	//void handleGame(int otherPort);
	TCPSocket* getSocket(){return sock;};
	char* getUsername(){return username;};
	char* getPassword(){return password;};
	char* getCmd(){return cmd;};
	/**
	 * This method send request to disconnect from the server
	 */
	void disconnectFromServer();
	/**
	 * This method login user to the server or register him, depends if cmd is 'login' or 'register'
	 */
	bool loginOrRegister();
	/**
	 * This method send to the server request for RSP game high scores
	 */
	void showHighScores();
	/**
	 * This method send to the server request for list of online users
	 */
	void showUsers();

	/**
	 * This method ask from the server to start RSP game, if <username> is null some user will be choose randomly
	 */
	int startGameWith(char* username);



	RSPGameClient(char* u,char* p,char* c);
	virtual ~RSPGameClient();
};

} /* namespace networkingLab */

#endif /* SRC_RSPGAMECLIENT_H_ */
