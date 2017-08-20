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

class RSPGameClient: public MThread {
	TCPSocket* sock;
	char* username;
	char* password;
	void setUserAvailability(bool flag);
	void handleGame(UDPSocket* gameSocket);
public:

	bool loginOrRegister(char* cmd);
	void showHighScores();
	void showUsers();
	void waitForGame();
	void startGameWith(char* username);


	void run();
	RSPGameClient(char* u,char* p);
	virtual ~RSPGameClient();
};

} /* namespace networkingLab */

#endif /* SRC_RSPGAMECLIENT_H_ */
