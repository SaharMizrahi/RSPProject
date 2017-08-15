/*
 * LoginHandler.cpp
 *
 *  Created on: Aug 9, 2017
 *      Author: user
 */

#include "LoginHandler.h"
#include<string.h>
#include <fstream>
#include <iostream>
#include "File.h"
#include "LoginProtocol.h"
namespace networkingLab {
void LoginHandler::handleClient(TCPSocket* sock) {
	if(sock!=NULL)
	{
		int size;
		char buffer[100];
		//read msg from new client
		sock->read((char*)&size, 4);
		sock->read(buffer,ntohl(size));
		//define if it's login or register
		char* cmd=strtok(buffer," ");
		//get details
		char* username=strtok(NULL," ");
		char* password=strtok(NULL," ");
		if(strcmp(cmd,"login")!=0&&strcmp(cmd,"register")!=0)//we got wrong command
		{
			char* msg1="-1";
			int size=htonl(sizeof(msg1));
			sock->write((char*)&size, 4);
			sock->write(msg1, size);
		}
		else//login / register
		{
			/**
			 * check if username + password is valid
			 */
			int res=checkUser(cmd, username, password);//
			int netRes=htonl(res);
			sock->write((char*)&netRes, 4);//return to user the result
			switch(res)
			{
				case LOGIN_APPROVED://success login
				case REGISTER_APPROVED://success register
					this->socketsHandler->addClient(username, sock);//cliant can play RSP game
					break;
				case WRONG_COMMAND_ERROR:
				case WRONG_PASSWORD_ERROR:
				case WRONG_USERNAME_ERROR:
				case USER_EXIST_ERROR:
					sock->close();//client should re-connect to the server with the right username and password
					break;
			}
		}
	}
}
void LoginHandler::run() {
	this->stopFlag=false;
	TCPSocket* readySock;
	while(!stopFlag&&(readySock=socket->listenAndAccept())!=NULL)//wait for new user to connect
	{
		this->handleClient(readySock);



	}

}

LoginHandler::LoginHandler(TCPSocket* listeningSocket,ConnectedSocketsHandler** sh) {
	socket=listeningSocket;
	this->socketsHandler=*sh;
	this->stopFlag=true;

}

int LoginHandler::checkUser(char* cmd,char* username, char* password) {
	ifstream in("src/users.txt");
	string line;
	if(in.is_open())
	{
		if(strcmp(cmd,"login")==0)//this is an exist client
		{
			while(getline(in,line)&&strcmp(line.c_str(),"")!=0)
			{
				//get each username and password from users file
				const char* s=line.c_str();
				char* str=new char[line.length()+1];
				strcpy(str,s);
				char* u=strtok(str," ");
				char* p=strtok(NULL," ");
				if(strcmp(u,username)==0)
				{
					if(strcmp(p,password)==0)//username exist,password match to username
					{
						in.close();
						return 0;
					}
					else//username exist,password don't match
					{
						in.close();
						return 1;
					}
				}
			}
			//there isn't user with this username
			return 2;
		}
		else if(strcmp(cmd,"register")==0)//this is a new client
		{
			while(getline(in,line)&&strcmp(line.c_str(),"")!=0)
			{
				const char* s=line.c_str();
				char* str=new char[line.length()+1];
				strcpy(str,s);
				char* u=strtok(str," ");
				if(strcmp(u,username)==0)//there is already an axist user with this username
				{
					return 4;
				}

			}
			//there isn't user with this username,so we update the users file and register the client
			if(in.is_open())
			{
				in.close();
				char buffer[100];
				strcat(buffer,username);
				strcat(buffer," ");
				strcat(buffer,password);
				ofstream out ("src/users.txt");//update users file
				out.write(buffer,sizeof(buffer));
				out.close();
				ofstream rankOut ("src/rank.txt");//init new user rank with 0
				char b[100];
				strcat(b,username);
				strcat(b," ");
				strcat(b,"0");
				rankOut.write(b,sizeof(b));
				rankOut.close();
				return 3;
			}
			else//there was error with register the client
			{
				return -2;
			}
		}
	}
	return -2;


}

void LoginHandler::stop() {
	this->stopFlag=true;
	this->waitForThread();
}



LoginHandler::~LoginHandler() {
	// TODO Auto-generated destructor stub
this->socket->close();

}

} /* namespace networkingLab */
