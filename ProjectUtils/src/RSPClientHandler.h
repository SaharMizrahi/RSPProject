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
	void updateUsersRank(TCPSocket* u1,char* res);
	/**
	 * This method define what the user want,and trigger relevant methods
	 */
	void analyzeClientMsg(TCPSocket* client);
	/**
	 * This method send to the client the game High Scores
	 */
	void showHighScores(TCPSocket* client);
	/**
	 * This method send to the client the online users list
	 */
	void showOnlineUsers(TCPSocket* client);
	/**
	 * This method start RSP game between two users, if <username> is null, the method will choose randomly available users
	 */
	void startGame(TCPSocket* client,char* username);
	/**
	 * if flag is true, the method will set the user available for game, if flag is false, the method will set the user not available
	 */
	void setUserAvailability(TCPSocket* sock,bool flag);
	/**
	 * This method diconnect user from RSP server
	 */
	void disconnectUserFromServer(TCPSocket* socket);
public:
	/**
	 * This method stop the listening thread
	 */
	void stopListening();
	/**
	 * This method update users vector
	 */
	void update(vector<RSPUser*>* updater);
	/**
	 * This method update sockets vector
	 */
	void update(vector<TCPSocket*> updater);
	virtual void run();
	RSPClientHandler(vector<TCPSocket*>* vec,vector<RSPUser*>* u);
	virtual ~RSPClientHandler();
};

} /* namespace networkingLab */

#endif /* SRC_RSPCLIENTHANDLER_H_ */
