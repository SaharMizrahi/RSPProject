/*
 * Client.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: user
 */

#include "Client.h"

namespace networkingLab {

Client::Client() {
	// TODO Auto-generated constructor stub
	this->gameSocket=NULL;
	this->identity=NULL;
	this->otherClientSocket=NULL;
	this->serverSocket=NULL;




}

Client::~Client() {
	// TODO Auto-generated destructor stub
}

} /* namespace networkingLab */
