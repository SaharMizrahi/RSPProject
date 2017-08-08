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
using namespace std;
namespace networkingLab {

class RSPGame: public MThread {
private:
	TCPSocket* clientA;
	TCPSocket* clientB;

public:
	virtual void run();
	RSPGame(Client* a,Client* b);
	virtual ~RSPGame();
};

} /* namespace networkingLab */

#endif /* SRC_RSPGAME_H_ */
