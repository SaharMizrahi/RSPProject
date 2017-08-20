/*
 * RSPClientHandler.cpp
 *
 *  Created on: Aug 17, 2017
 *      Author: user
 */

#include "RSPClientHandler.h"
#include "LoginProtocol.h"
#include <string.h>
#include <fcntl.h>
#include <fstream>

namespace networkingLab {

void RSPClientHandler::update(vector<RSPUser*>* updater) {
	this->users=updater;
}

RSPClientHandler::RSPClientHandler(vector<TCPSocket*> vec,vector<RSPUser*>* u) {
	// TODO Auto-generated constructor stub
	this->listener=new MultipleTCPSocketsListener();
	this->socketVector=&vec;
	listener->addSockets(vec);
	this->stopListen=true;
	this->users=u;

}

void RSPClientHandler::update(vector<TCPSocket*> updater) {
	this->listener->interupt();
	this->socketVector=&updater;
	this->listener->addSockets(updater);
	this->listener->interupt();


}

void RSPClientHandler::analyzeClientMsg(TCPSocket* client) {

	if(client!=NULL)
	{
		char buffer[100];
		memset(buffer,0,100);
		int msgSize;
		client->read((char*)&msgSize, 4);
		client->read(buffer, msgSize);
		char* cmd=strtok(buffer," ");
		int c=atoi(cmd);
		switch(c)
		{
		case 1:
			showOnlineUsers(client);
			break;
		case 2:
			showHighScores(client);
			break;
		case 3:

			startGame(client,strtok(NULL," ") );
			break;
		case 4:
			startGame(client, NULL);
			break;

		}

	}
}
void RSPClientHandler::showHighScores(TCPSocket* client) {
	ifstream in("src/rank.txt");
	if(in.is_open())
	{
		char buffer[100]="\n***High Scores:***\n";

		string line;
		while(getline(in,line)&&strcmp(line.c_str(),"")!=0)
		{
			strcat(buffer,line.c_str());
			strcat(buffer,"\n");

		}
		int size=strlen(buffer)+1;
		client->write((char*)&size, 4);
		client->write(buffer, size);
	}
	else
	{
		char b[]="Error";
		int s=htonl(strlen(b)+1);
		client->write((char*)&s, 4);
		client->write(b, s);
		cout<<"can't open rank.txt"<<endl;
	}

}

void RSPClientHandler::showOnlineUsers(TCPSocket* client) {
	if(client!=NULL)
	{
		char buffer[1000];
		memset(buffer,0,100);
		for(int i=0;i<users->size();i++)
		{
			strcat(buffer,users->at(i)->getUsername());
			if(users->at(i)->isAvailable())
				strcat(buffer," available ");
			else
				strcat(buffer," not-available ");



		}
		int s=strlen(buffer)+1;
		client->write((char*)&s, 4);
		client->write(buffer, s);

	}


}

void RSPClientHandler::startGame(TCPSocket* client, char* username) {
	if(client!=NULL)
	{
		if(username!=NULL)
		{

		}
		else//random user
		{

		}
	}
}

void RSPClientHandler::run() {
	stopListen=false;
	char buffer[100];

	TCPSocket* client=NULL;
	while(!stopListen )
	{

		client=listener->listenToSocket();
		if(client!=NULL)
		{
			analyzeClientMsg(client);
			/*memset(buffer,0,100);
			int msgSize;
			client->read((char*)&msgSize, 4);
			client->read(buffer, msgSize);
			cout<<"got msg: "<<buffer<<endl;
			char answer[]="yalla";
			int s=strlen(answer)+1;
			client->write((char*)&s, 4);
			client->write(answer, s);
			sleep(1);*/



		}
		client=NULL;
	}

}

RSPClientHandler::~RSPClientHandler() {
	// TODO Auto-generated destructor stub

}


void RSPClientHandler::stopListening() {
	this->stopListen=true;
	if(listener!=NULL)
		listener->interupt();
}



} /* namespace networkingLab */

