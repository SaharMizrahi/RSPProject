/*
 * RSPGame.h
 *
 *  Created on: Aug 8, 2017
 *      Author: user
 */

#ifndef SRC_RSPGAME_H_
#define SRC_RSPGAME_H_
#include<iostream>
#include<stdio.h>
#include "MThread.h"
#include "TCPSocket.h"
#include "Client.h"
using namespace std;
namespace networkingLab {

class RSPGame: public MThread {
private:
	TCPSocket* clientA;
	TCPSocket* clientB;
	char winner;

public:
	char getWinner(){return winner;};
	int whoWon(string a,string b);//this is our game policy
	virtual void run();
	RSPGame(Client* a,Client* b);
	virtual ~RSPGame();
};

} /* namespace networkingLab */

#endif /* SRC_RSPGAME_H_ */
