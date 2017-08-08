/*
 * User.h
 *
 *  Created on: Aug 8, 2017
 *      Author: user
 */

#ifndef SRC_USER_H_
#define SRC_USER_H_
#include<iostream>
#include<stdio.h>

using namespace std;
namespace networkingLab {

class User {
private:
	string username;
	string password;
	int rank;
	bool availability;
public:
	/***getters and setters****/
	void setRank(int r){rank=r;};
	void setAvailability(bool a){availability=a;};
	void setNewPassword(string newPass){password=newPass;};//if we want we can set guidelines to valid password
	string getUsername(){return username;};
	string getPassword(){return password;};
	int getRank(){return rank;};
	bool isAvailable(){return availability;};
	User(string name,string pass);
	virtual ~User();
};

} /* namespace networkingLab */

#endif /* SRC_USER_H_ */
