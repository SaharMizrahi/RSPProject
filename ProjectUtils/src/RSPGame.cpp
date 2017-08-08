/*
 * RSPGame.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: user
 */
#define IP "127.0.0.1"
#define portA 6666
#define portB 7777

#include "RSPGame.h"

namespace networkingLab {

RSPGame::RSPGame(Client* a,Client* b) {
	// TODO Auto-generated constructor stub
	this->clientA=new TCPSocket(portA);//bind to client a
	this->clientB=new TCPSocket(portB);//bind to client b
	a->setGameSocket(IP,portA);
	b->setGameSocket(IP, portB);

}

RSPGame::~RSPGame() {
	// TODO Auto-generated destructor stub
}

} /* namespace networkingLab */
