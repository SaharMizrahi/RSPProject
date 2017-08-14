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
using namespace std;

namespace networkingLab {

class Game : public MThread{
	RSPUser* user1;
	RSPUser* user2;
	void startGame();
	int policy(char* c1,char* c2);
public:
	void run();
	Game(RSPUser* u1,RSPUser* u2);
	virtual ~Game();
};

} /* namespace networkingLab */

#endif /* SRC_GAME_H_ */
