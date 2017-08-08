/*
 * Client.h
 *
 *  Created on: Aug 8, 2017
 *      Author: user
 */

#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_
#include<iostream>
#include<stdio.h>
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "User.h"
using namespace std;
namespace networkingLab {

class Client {
	User* identity;
	TCPSocket* serverSocket;//sync
	TCPSocket* gameSocket;//sync
	UDPSocket* otherClientSocket;//Async

public:

	Client();
	virtual ~Client();
};

} /* namespace networkingLab */

#endif /* SRC_CLIENT_H_ */
