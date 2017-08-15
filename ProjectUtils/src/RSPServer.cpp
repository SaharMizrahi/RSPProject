/*
 * RSPServer.cpp
 *
 *  Created on: Aug 9, 2017
 *      Author: user
 */

#include "RSPServer.h"
#include "LoginHandler.h"
namespace networkingLab {

RSPServer::RSPServer(int port) {
	// TODO Auto-generated constructor stub
	 this->port=port;
	 sh=NULL;
	 lh=NULL;
}

void RSPServer::runServer() {

	TCPSocket* loginSocket=new TCPSocket(this->port);
	sh=new ConnectedSocketsHandler();
	lh=new LoginHandler(loginSocket,&this->sh);
	lh->start();
}

RSPServer::~RSPServer() {
	// TODO Auto-generated destructor stub
	lh->stop();


}

} /* namespace networkingLab */
