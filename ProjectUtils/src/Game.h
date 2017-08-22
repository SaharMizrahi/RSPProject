/*
 * Game.h
 *
 *  Created on: Aug 13, 2017
 *      Author: user
 */

#ifndef SRC_GAME_H_
#define SRC_GAME_H_
#include<iostream>
#include<stdio.h>
#include "MThread.h"
#include "RSPUser.h"
#include "UDPSocket.h"
#include "Protocol.h"
using namespace std;

namespace networkingLab {

class Game : public MThread{
	RSPUser* user1;
	RSPUser* user2;
	/**
	 * This method start RSP game between two users
	 */
	void startGame();
	/**
	 * This method check who won in the RSP game. 1-u1 0-draw -1-u2
	 *
	 * */
	int policy(char* c1,char* c2);
	/**
	 * This method handle every round of RSP game
	 */
	int handleRound(UDPSocket* client1,UDPSocket* client2);
public:
	/*
	 * This method manage the game and run on a different thread
	 * **/
	void run();
	Game(RSPUser* u1,RSPUser* u2);
	virtual ~Game();
};

} /* namespace networkingLab */

#endif /* SRC_GAME_H_ */
