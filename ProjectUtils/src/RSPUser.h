/*
 * RSPUser.h
 *
 *  Created on: Aug 12, 2017
 *      Author: user
 */

#ifndef SRC_RSPUSER_H_
#define SRC_RSPUSER_H_
#include "TCPSocket.h"
namespace networkingLab {

class RSPUser {
	char * username;
	int rank;
	TCPSocket* socket;
	int port;
public:
	char* getUsername(){return username;};
	int getRank(){return rank;};
	TCPSocket* getSocket(){return socket;};
	RSPUser(char* u,int r,TCPSocket* s,int p);
	virtual ~RSPUser();
};

} /* namespace networkingLab */

#endif /* SRC_RSPUSER_H_ */
