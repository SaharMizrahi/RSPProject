/*
 * RSPUser.cpp
 *
 *  Created on: Aug 12, 2017
 *      Author: user
 */

#include "RSPUser.h"

namespace networkingLab {

RSPUser::RSPUser(char* u,int r,TCPSocket* s,int p) {
	// TODO Auto-generated constructor stub
	this->username=u;
	this->rank=r;
	this->socket=s;
	this->port=p;

}

RSPUser::~RSPUser() {
	// TODO Auto-generated destructor stub
}

} /* namespace networkingLab */
