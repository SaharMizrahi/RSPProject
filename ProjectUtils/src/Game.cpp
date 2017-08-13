/*
 * Game.cpp
 *
 *  Created on: Aug 13, 2017
 *      Author: user
 */

#include "Game.h"

namespace networkingLab {

Game::Game(RSPUser* u1,RSPUser* u2) {
	// TODO Auto-generated constructor stub
	this->user1=u1;
	this->user2=u2;

}
void Game::startGame() {


}

void Game::run() {
	if(user1!=NULL&&user2!=NULL)
	{
		startGame();
	}

}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

} /* namespace networkingLab */

