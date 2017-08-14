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
	this->availableForGame=false;

}

void RSPUser::setNewRank(int newRank) {
	if(newRank>=0)
	{
		rank=newRank;
	}
}
RSPUser::~RSPUser() {
	// TODO Auto-generated destructor stub
}

} /* namespace networkingLab */


