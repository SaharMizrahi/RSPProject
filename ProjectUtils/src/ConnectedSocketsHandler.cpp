/*
 * ConnectedSocketsHandler.cpp
 *
 *  Created on: Aug 11, 2017
 *      Author: user
 */

#include "ConnectedSocketsHandler.h"
#include <fstream>
#include <string.h>
#include "RSPUser.h"
#include "Game.h"
#include <stdlib.h>     /* srand, rand */

namespace networkingLab {

ConnectedSocketsHandler::ConnectedSocketsHandler() {
	// TODO Auto-generated constructor stub
	this->listener=new MultipleTCPSocketsListener();
	this->listener->addSockets(this->serverSockets);
	this->stop=true;
}

void ConnectedSocketsHandler::handleClientMsg(TCPSocket* socket) {
	int size;
	char buffer[100];
	//read msg from user
	socket->read((char*)&size,4);
	socket->read(buffer, ntohl(size));
	const char* s=buffer;
	char* str=new char[sizeof(buffer)+1];
	strcpy(str,s);
	char* cmd=strtok(str," ");
	if(strcmp(cmd,"")!=0)
	{
		if(strcmp(cmd,"1")==0)
		{
			this->showUsers(socket);
		}
		else if(strcmp(cmd,"2")==0)
			this->showHS(socket);
		else if(strcmp(cmd,"3")==0)
		{
			char* username=strtok(NULL," ");
			this->startGame(socket, username);
		}
		else if (strcmp(cmd,"4")==0)
			this->startGame(socket, NULL);
		else if (strcmp(cmd,"5")==0)
		{
			this->setUserAvailable(socket);
		}
		else if (strcmp(cmd,"6")==0)
		{
			this->setUserUnAvailable(socket);
		}
		else if(strcmp(cmd,"7")==0)
		{
			this->disconnectUser(socket);

		}


		this->stop=true;
		this->listener->addSocket(socket);
		this->stop=false;
	}




}
void ConnectedSocketsHandler::run() {
	TCPSocket* sock;
	stop=false;
	while((sock=this->listener->listenToSocket())!=NULL&&!stop)//wait for incoming msg from connected user
	{
		stop=true;
		this->listener->removeSocket(sock);
		this->handleClientMsg(sock);
		stop=false;


	}


}

void ConnectedSocketsHandler::addClient(char* username, TCPSocket* socket) {
	int rank=0;
	ifstream in ("src/rank.txt");//get user rank,if we don;t find init it by 0
	if(in.is_open())
	{
		string line;
		while(getline(in,line)&&strcmp(line.c_str(),"")!=0)
		{
			const char* s=line.c_str();
			char* str=new char[line.length()+1];
			strcpy(str,s);
			char* u=strtok(str," ");
			char* r=strtok(NULL," ");
			if(strcmp(u,username)==0)
			{
				rank=atoi(r);

				break;
			}
		}

	}
	//random a new port for the new client so he will connect to other socket
	int random=rand()%100+1;
	int newPort=random+socket->getPort();
	int netPort=htonl(newPort);
	socket->write((char*)&netPort, 4);
	socket=new TCPSocket(newPort);
	TCPSocket* ready=socket->listenAndAccept();//wait for the client to connect to the new socket
	RSPUser* newUser=new RSPUser(username,rank,ready,newPort);
	//update vectors
	this->usersVector.push_back(newUser);
	this->stop=true;
	this->listener->addSocket(ready);
	this->start();
}

void ConnectedSocketsHandler::showHS(TCPSocket* socket) {
	ifstream in("src/rank.txt");
	if(in.is_open())
	{
		char buffer[100]="High Scores:\n";

		string line;
		while(getline(in,line)&&strcmp(line.c_str(),"")!=0)
		{
			strcat(buffer,line.c_str());
			strcat(buffer,"\n");

		}
		cout<<"connected:\n"<<buffer<<endl;
		int size=htonl(strlen(buffer)+1);
		socket->write((char*)&size, 4);
		socket->write(buffer, size);
	}
}

void ConnectedSocketsHandler::showUsers(TCPSocket* socket) {
	char buffer[1000]="Users List:\n";
	for(unsigned int i=0;i<this->usersVector.size();i++)
	{
		strcat(buffer,usersVector[i]->getUsername());
		if(usersVector[i]->isAvailable())
			strcat(buffer," available for game");
		else
			strcat(buffer," not available for game");
		strcat(buffer," \n");
	}
	int size=htonl(strlen(buffer)+1);
	socket->write((char*)&size, 4);
	socket->write(buffer, size);

}

void ConnectedSocketsHandler::startGame(TCPSocket* inviter, char* invitedUser) {

	RSPUser* user1=NULL;
	RSPUser* user2=NULL;
	for(unsigned int i=0;i<this->usersVector.size();i++)
	{
		if(usersVector[i]->getSocket()->getFd()==inviter->getFd())//finding user1
			user1=usersVector[i];
		if(invitedUser!=NULL)
		{
		if(strcmp(usersVector[i]->getUsername(),invitedUser)==0)//finsing user2
			user2=usersVector[i];

		}
		else if(usersVector[i]->getSocket()->getFd()!=inviter->getFd()&&usersVector[i]->isAvailable())//in case of random choice, it has to be not the inviter and also available
		{
			user2=usersVector[i];

		}

	}
	if(user2==NULL)//we don't found a match or there isn't available user
	{
		char * msg="failed";
		int size=htonl(sizeof(msg));
		inviter->write((char*)&size, 4);
		inviter->write(msg, size);
		return;
	}
	if(user1!=user2)
	{
		//need to send udp ports for user1 and user 2 for playing the game
		user1->setAvailability(false);
		user2->setAvailability(false);
		char* msg="start";
		int size=htonl(sizeof(msg));
		user1->getSocket()->write((char*)&size,4);
		user1->getSocket()->write(msg, size);
		user2->getSocket()->write((char*)&size,4);
		user2->getSocket()->write(msg, size);
		Game* newGame=new Game(user1,user2);
		newGame->start();
		return;

	}


}


void ConnectedSocketsHandler::setUserAvailable(TCPSocket* socket) {
	for(unsigned int i=0;i<this->usersVector.size();i++)
		if(usersVector[i]->getSocket()->getFd()==socket->getFd())
		{
			usersVector[i]->setAvailability(true);
			break;
		}
}

void ConnectedSocketsHandler::disconnectUser(TCPSocket* socket) {
	int index;
	for(unsigned int i=0;i<this->usersVector.size();i++)//delete fro, users vector
		if(usersVector[i]->getSocket()->getFd()==socket->getFd())
		{
			index=i;
			break;
		}
	this->usersVector.erase(usersVector.begin()+index);
	this->listener->removeSocket(socket);


}

void ConnectedSocketsHandler::setUserUnAvailable(TCPSocket* socket) {

	for(unsigned int i=0;i<this->usersVector.size();i++)
		if(usersVector[i]->getSocket()->getFd()==socket->getFd())
		{
			usersVector[i]->setAvailability(false);
			break;
		}
}

ConnectedSocketsHandler::~ConnectedSocketsHandler() {


	// TODO Auto-generated destructor stub
}

} /* namespace networkingLab */
