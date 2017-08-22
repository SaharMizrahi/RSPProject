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

	if(user1!=NULL&user2!=NULL)
	{
		//announcing to the users that game is ready to go
		bool ok1=false,ok2=false;
		char* msg="ok";
		int size=sizeof(msg);
		TCPSocket* socket=user1->getSocket();
		socket->write((char*)&size, 4);
		socket->write(msg, size);
		socket=user2->getSocket();
		socket->write((char*)&size, 4);
		socket->write(msg, size);
		UDPSocket* sock1=new UDPSocket(user1->getPort());
		UDPSocket* sock2=new UDPSocket(user2->getPort());
		char buffer[10];
		memset(buffer,0,10);
		int s;
		//wait for ack from two users
		sock1->read((char*)&s, 4);
		sock1->read(buffer, s);
		if(strcmp(buffer,"ack")==0)
		{
			ok1=true;
		}
		else
		{
			ok1=false;
		}
		memset(buffer,0,10);
		sock2->read((char*)&s, 4);
		sock2->read(buffer, s);

		if(strcmp(buffer,"ack")==0)
		{
			ok2=true;
		}
		else
		{
			ok2=false;
		}
		if(ok1&&ok2)
		{
			char* msg="start";
			int size=sizeof(msg);
			sock1->write((char*)&size, 4);
			sock1->write(msg, size);
			sock2->write((char*)&size, 4);
			sock2->write(msg, size);
			sleep(1);
			int res=0;
			int sum=0;
			int rounds=0;
			while(rounds<3||sum==0)
			{
				res=this->handleRound(sock1, sock2);
				switch(res)
				{
				case USER1WON:
					sum+=1;
					rounds++;
					break;
				case USER2WON:
					sum+=-1;
					rounds++;
					break;
				case DRAW:
					rounds++;
					break;
				case USER1CANCLE:
					sum=-3;
					rounds=3;
					break;
				case USER2CANCLE:
					sum=3;
					rounds=3;
					break;
				case BOTHCANCLE:
					rounds=3;
					sum=10;
					break;
				}
			}
			char buf[100];
			memset(buf,0,100);
			strcat(buf,"Finish : ");
			if(sum==10)//they both cancel
			{
				strcat(buf,"both clients failed");
				int bufSize=sizeof(buf);
				sock1->write((char*)&bufSize, 4);
				sock2->write((char*)&bufSize, 4);
				sock1->write(buf, bufSize);
				sock2->write(buf, bufSize);
				return;
			}
			else if(sum>0)//user1 won
			{

				strcat(buf,this->user1->getUsername());
			}
			else if(sum<0)//user2 won
			{
				strcat(buf,this->user2->getUsername());

			}
			strcat(buf," won");
			int bufSize=sizeof(buf);
			sock1->write((char*)&bufSize, 4);
			sock2->write((char*)&bufSize, 4);
			sock1->write(buf, bufSize);
			sock2->write(buf, bufSize);
			/**
			 * UPDATE RANK FILE
			 */


			if(sock1!=NULL)
				sock1->close();
			if(sock2!=NULL)
				sock2->close();

		}
		else//announce to the users that the game is canceled
		{
			char* msg="Error";
			int size=sizeof(msg);
			sock1->write((char*)&size, 4);
			sock1->write(msg, size);
			sock2->write((char*)&size, 4);
			sock2->write(msg, size);
			sleep(2);
			sock1->close();
			sock2->close();
		}





	}

}
int Game::handleRound(UDPSocket* client1, UDPSocket* client2)
{
	char menu[]="\n***RSP GAME MENU***\nchoose one of the options:\nr-rock\ns-scissors\np-paper\nc-close\nundefined answer will cause you official lose! ";
	int menuSize=sizeof(menu);
	client1->write((char*)&menuSize, 4);
	client1->write(menu, menuSize);
	client2->write((char*)&menuSize, 4);
	client2->write(menu, menuSize);

	char buf1[10],buf2[10];
	int s1,s2;
	client1->read((char*)&s1, 4);
	client2->read((char*)&s2, 4);
	client1->read(buf1, s1);
	client2->read(buf2, s1);

	return policy(buf1, buf2);





	return 0;
}
int Game::policy(char* c1, char* c2) {

	if(c1!=NULL&&c2!=NULL)
	{
		if(strcmp(c1,"r")!=0&&strcmp(c1,"s")!=0&&strcmp(c1,"p")!=0)//client 1 is going to lose the game
		{
			if(strcmp(c2,"r")!=0&&strcmp(c2,"s")!=0&&strcmp(c2,"p")!=0)//both client 2
			{
				return BOTHCANCLE;
			}
			else
				return USER1CANCLE;
		}
		else if(strcmp(c2,"r")!=0&&strcmp(c2,"s")!=0&&strcmp(c2,"p")!=0)
			return USER2CANCLE;
		else if(strcmp(c1,c2)==0)
			return DRAW;
		else if(strcmp(c1,"rock")==0)
		{
			if(strcmp(c2,"paper")==0)
				return USER2WON;
			else
				return USER1WON;
		}
		else if(strcmp(c1,"scissors")==0)
		{
			if(strcmp(c2,"rock")==0)
				return USER2WON;
			else
				return USER1WON;
		}
		else if(strcmp(c1,"paper")==0)
		{
			if(strcmp(c2,"scissors")==0)
				return USER2WON;
			else
				return USER1WON;
		}

	}
	else
		return BOTHCANCLE;
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


