/*
 * RSPServer.h
 *
 *  Created on: Aug 9, 2017
 *      Author: user
 */

#ifndef SRC_RSPSERVER_H_
#define SRC_RSPSERVER_H_

#include<iostream>
#include<stdio.h>
#include "TCPSocket.h"
#include <pthread.h>
#include "MThread.h"
#include "LoginHandler.h"
#include <vector>
#include "MultipleTCPSocketsListener.h"
#include "ConnectedSocketsHandler.h"
using namespace std;
namespace networkingLab {


class RSPServer {
private:
	int port;
	LoginHandler *lh;
	ConnectedSocketsHandler* sh;
public:
	void runServer();
	RSPServer(int port);
	virtual ~RSPServer();
};


} /* namespace networkingLab */

#endif /* SRC_RSPSERVER_H_ */
