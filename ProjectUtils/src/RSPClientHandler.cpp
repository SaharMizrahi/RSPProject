/*
 * RSPClientHandler.cpp
 *
 *  Created on: Aug 17, 2017
 *      Author: user
 */

#include "RSPClientHandler.h"
#include <string.h>
#include <fcntl.h>
#include <fstream>
#include "Protocol.h"
#include <sstream>  // Required for stringstreams

namespace networkingLab {

void RSPClientHandler::update(vector<RSPUser*>* updater) {
	this->users=updater;
}

RSPClientHandler::RSPClientHandler(vector<TCPSocket*>* vec,vector<RSPUser*>* u) {
	// TODO Auto-generated constructor stub
	this->listener=new MultipleTCPSocketsListener();
	this->socketVector=vec;
	listener->addSockets(*vec);
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
		case SHOW_USERS:
			showOnlineUsers(client);
			break;
		case SHOW_HS:
			showHighScores(client);
			break;
		case START_GAME_WITH_USER:

			startGame(client,strtok(NULL," ") );
			break;
		case START_GAME_WITH_RANDOM:
			startGame(client, NULL);
			break;
		case SET_USER_AVAILABLE:
			setUserAvailability(client, true);
			break;
		case SET_USER_UNAVAILABLE:
			setUserAvailability(client, false);
			break;
		case DISCONNECT:
			disconnectUserFromServer(client);
			break;
		case GAME_END:
			updateUsersRank(client,strtok(NULL," ") );
			setUserAvailability(client, false);



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
	RSPUser *u1=NULL,*u2=NULL;

	if(client!=NULL)
	{
		//find user1
		for(int i=0;i<users->size();i++)
		{
			if(client->getFd()==users->at(i)->getSocket()->getFd())
			{
				u1=users->at(i);
				break;

			}

		}
		//find user2
		for(int i=0;i<users->size();i++)
		{
			if(username!=NULL)
			{

				if(strcmp(username,users->at(i)->getUsername())==0)
				{
					u2=users->at(i);
					break;
				}
			}
			else
			{

				if(users->at(i)->isAvailable())
				{
					u2=users->at(i);
					break;
				}
			}
		}
		//start game
		if(u1!=NULL&&u2!=NULL&&u1->getSocket()->getFd()!=u2->getSocket()->getFd())
		{
			int p1=u1->getSocket()->getPort(),p2=u2->getSocket()->getPort();

			u1->getSocket()->write((char*)&p2, 4);
			u2->getSocket()->write((char*)&p1, 4);
			this->setUserAvailability(u1->getSocket(), false);
			this->setUserAvailability(u2->getSocket(), false);



		}
		else
		{
			int num=-1;
			if(u1!=NULL)
				u1->getSocket()->write((char*)&num, 4);
			if(u2!=NULL)
				u2->getSocket()->write((char*)&num, 4);
		}





	}
}

void RSPClientHandler::setUserAvailability(TCPSocket* sock, bool flag) {
	if(sock!=NULL)
	{
		for(int i=0;i<this->users->size();i++)
		{
			if(users->at(i)->getSocket()->getFd()==sock->getFd())
			{
				users->at(i)->setAvailability(flag);
				if(flag)
					cout<<users->at(i)->getUsername()<<" is available"<<endl;
				else
					cout<<users->at(i)->getUsername()<<" is un-available"<<endl;

				break;
			}
		}
	}
}

void RSPClientHandler::disconnectUserFromServer(TCPSocket* socket) {
	if(socket!=NULL)
	{
		char* username;
		int rank;
		//remove from users vector
		for(int i=0;i<this->users->size();i++)
		{
			if(users->at(i)->getSocket()->getFd()==socket->getFd())
			{
				username=users->at(i)->getUsername();
				rank=users->at(i)->getRank();
				cout<<users->at(i)->getUsername()<<" has left the RSP server"<<endl;
				users->erase(users->begin()+i);

				break;
			}
		}
		//update rank
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



		for(int j=0;j<this->socketVector->size();j++)
		{
			if(socketVector->at(j)->getFd()==socket->getFd())
			{
				socketVector->erase(socketVector->begin()+j);
				listener->interupt();
				listener->addSockets(*socketVector);
				listener->interupt();
				break;

			}

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

void RSPClientHandler::updateUsersRank(TCPSocket* u1, char* res) {
	char* username;
	int newRank;
	cout<<"change rank "<<res<<endl;
	if(strcmp(res,"draw")==0||strcmp(res,"win")==0)
	{
		cout<<"1"<<endl;
		for(int i=0;i<this->users->size();i++)
		{
			if(users->at(i)->getSocket()->getFd()==u1->getFd())
			{
				cout<<"2"<<endl;

				username=users->at(i)->getUsername();
				newRank=users->at(i)->getRank();
				if(strcmp(res,"draw")==0)
				{
					newRank+=1;
				}
				else if(strcmp(res,"win")==0)
				{
					newRank+=3;

				}
				//update rank.txt
				ifstream in ("src/rank.txt");
				if(in.is_open())
				{
					cout<<"3"<<endl;

					char buffer[100];
					memset(buffer,0,100);
					string line;
					while(getline(in,line)&&strcmp(line.c_str(),"")!=0)
					{
						strcat(strcat(buffer,line.c_str())," ");
					}
					in.close();
					ofstream out;
					out.open("src/rank.txt", std::ofstream::out | std::ofstream::trunc);
					cout<<"4"<<endl;

					char* s=strtok(buffer," ");
					while(s!=NULL)
					{
						if(strcmp(s,username)==0)
						{
							out.write(s,strlen(s));
							out.write(" ",1);
							std::ostringstream oss;
						    oss<< newRank;
						    out.write(oss.str().c_str(),strlen(oss.str().c_str()));
							out.write("\n",1);
							s=strtok(NULL," ");//read and throw the last rank
						}
						else
						{
							out.write(s,strlen(s));
							out.write(" ",1);
							s=strtok(NULL," ");
							out.write(s,strlen(s));
							out.write("\n",1);
						}
						s=strtok(NULL," ");

					}

					out.close();

				}

				return;
			}
		}
	}
}

} /* namespace networkingLab */


