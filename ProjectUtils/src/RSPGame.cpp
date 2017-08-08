/*
 * RSPGame.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: user
 */
#define IP "127.0.0.1"
#define portA 6666
#define portB 7777
#define r "rock"
#define s "scissors"
#define p "paper"

#include "RSPGame.h"

namespace networkingLab {

RSPGame::RSPGame(Client* a,Client* b) {
	// TODO Auto-generated constructor stub
	this->clientA=new TCPSocket(portA);//bind to client a
	this->clientB=new TCPSocket(portB);//bind to client b
	a->setGameSocket(IP,portA);
	b->setGameSocket(IP, portB);

}

void RSPGame::run() {
	//run 3 times

	//get clientA choice

	//get clientB choice

	//define who won

	//update clientA's and clientB's rank

	//close connection to clientA

	//close connection to clientB




}

int RSPGame::whoWon(string a, string b) {
	if(a==b)//they choose the same choice
		return 0;
	else if(a==r)
	{
		if(b==p)
			return -1;
		else
			return 1;
	}
	else if(a==s)
	{
		if(b==p)
			return 1;
		else
			return -1;
	}
	else//a==paper
	{
		if(b==r)
			return 1;
		else
			return -1;
	}
}

RSPGame::~RSPGame() {
	// TODO Auto-generated destructor stub

}

} /* namespace networkingLab */
