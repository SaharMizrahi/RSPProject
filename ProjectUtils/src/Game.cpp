/*
 * Game.cpp
 *
 *  Created on: Aug 13, 2017
 *      Author: user
 */

#include "Game.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include<iostream>
#include<stdio.h>
#include <cstring>

using namespace std;
namespace networkingLab {

Game::Game(RSPUser* u1,RSPUser* u2) {
	// TODO Auto-generated constructor stub
	this->user1=u1;
	this->user2=u2;

}
void Game::startGame() {
	UDPSocket* sock1=new UDPSocket(user1->getPort());
	UDPSocket* sock2=new UDPSocket(user2->getPort());
	int win=0,res;
	int rounds=0;
	char buf1[10];
	char buf2[10];
	int b1=0,b2=0;
	while(rounds<3||win==0)
	{
		//wait for users choice rock/scissors/paper/exit
		sock1->read((char*)&b1, 4);
		sock2->read((char*)&b2, 4);
		sock1->read(buf1, ntohl(b1));
		sock2->read(buf2, ntohl(b2));
		if(buf1!=NULL&&buf2!=NULL)
		{
			if(strcmp(buf1,"exit")==0||strcmp(buf2,"exit")==0)//some user exit the game
			{
				if(strcmp(buf1,"exit")==0&&strcmp(buf2,"exit")==0)//they both exit the game
					break;//end game
				else if(strcmp(buf1,"exit")==0)//user1 exit the game
				{
					user1->setNewRank(user1->getRank()-1);
					user2->setNewRank(user2->getRank()+3);
					break;
				}
				else if(strcmp(buf2,"exit")==0)//user2 exit the game
				{
					user2->setNewRank(user2->getRank()-1);
					user1->setNewRank(user1->getRank()+3);
					break;
				}
			}
			else
			{
				res=policy(buf1,buf2);
				win+=res;
				if(res==0)
				{
					char buffer[10]="draw";
					int size=htonl(strlen(buffer)+1);
					sock1->write((char*)&size, 4);
					sock2->write((char*)&size, 4);
					sock1->write(buffer,size);
					sock2->write(buffer,size);
				}
				else if(res>0)
				{
					char buffer[10]="user1 won";
					int size=htonl(strlen(buffer)+1);
					sock1->write((char*)&size, 4);
					sock2->write((char*)&size, 4);
					sock1->write(buffer,size);
					sock2->write(buffer,size);
				}
				else if(res<0)
				{
					char buffer[10]="user2 won";
					int size=htonl(strlen(buffer)+1);
					sock1->write((char*)&size, 4);
					sock2->write((char*)&size, 4);
					sock1->write(buffer,size);
					sock2->write(buffer,size);
				}
				rounds++;
			}
		}
	}
	char buffer[10]="end";
	int size=htonl(strlen(buffer)+1);
	sock1->write((char*)&size, 4);
	sock2->write((char*)&size, 4);
	sock1->write(buffer,size);
	sock2->write(buffer,size);

	if(win>0)//user1 won
	{
		user1->setNewRank(user1->getRank()+3);
	}
	else if(win<0)//user2 won
	{
		user2->setNewRank(user2->getRank()+3);

	}
	user1->setAvailability(true);
	user2->setAvailability(true);
}
int Game::policy(char* c1, char* c2) {

	if(c1!=NULL&&c2!=NULL)
	{
		if(strcmp(c1,c2)==0)
			return 0;
		else if(strcmp(c1,"rock")==0)
		{
			if(strcmp(c2,"paper")==0)
				return -1;
			else
				return 1;
		}
		else if(strcmp(c1,"scissors")==0)
		{
			if(strcmp(c2,"rock")==0)
				return -1;
			else
				return 1;
		}
		else if(strcmp(c1,"paper")==0)
		{
			if(strcmp(c2,"scissors")==0)
				return -1;
			else
				return 1;
		}
		else//it's not rock/scissors/paper/exit
		{
			return 0;
		}
	}
	return 0;
}
void Game::run() {
	if(user1!=NULL&&user2!=NULL)
	{
		startGame();
	}

}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

} /* namespace networkingLab */


