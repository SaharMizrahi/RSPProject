/*
 * User.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: user
 */

#include "User.h"

namespace networkingLab {

User::User(string name,string pass) {
	// TODO Auto-generated constructor stub
	this->username=name;
	this->password=pass;
	this->rank=0;
	this->availability=false;


}



User::~User() {
	// TODO Auto-generated destructor stub
}

} /* namespace networkingLab */
