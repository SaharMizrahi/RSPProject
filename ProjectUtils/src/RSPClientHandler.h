/*
 * RSPClientHandler.h
 *
 *  Created on: Aug 17, 2017
 *      Author: user
 */

#ifndef SRC_RSPCLIENTHANDLER_H_
#define SRC_RSPCLIENTHANDLER_H_
#include "MThread.h"
#include<iostream>
#include<stdio.h>
#include "MultipleTCPSocketsListener.h"
#include "vector"
#include "TCPSocket.h"
#include "RSPUser.h"
using namespace std;
namespace networkingLab {

class RSPClientHandler:public MThread {
	MultipleTCPSocketsListener* listener;
	vector<TCPSocket*>* socketVector;
	vector<RSPUser*>* users;
	bool stopListen;
	void analyzeClientMsg(TCPSocket* client);
	void showHighScores(TCPSocket* client);
	void showOnlineUsers(TCPSocket* client);
	void startGame(TCPSocket* client,char* username);

public:
	void stopListening();
	void update(vector<RSPUser*>* updater);
	void update(vector<TCPSocket*> updater);
	void run();
	RSPClientHandler(vector<TCPSocket*> vec,vector<RSPUser*>* u);
	virtual ~RSPClientHandler();
};

} /* namespace networkingLab */

#endif /* SRC_RSPCLIENTHANDLER_H_ */
