/*
 * RSPGameClient.cpp
 *
 *  Created on: Aug 15, 2017
 *      Author: user
 */

#include "RSPGameClient.h"
#include <string.h>
#include "LoginProtocol.h"
#include "ServerProtocol.h"
namespace networkingLab {

RSPGameClient::RSPGameClient() {
	// TODO Auto-generated constructor stub.

	this->socket=NULL;
	this->endOfSession=false;

}
void RSPGameClient::startInterface() {
	do{
	cout<<"MENU:"<<endl;
	cout<<"login <username> <password>"<<endl;
	cout<<"register <username> <password>"<<endl;
	cout<<"exit game"<<endl;
	char buffer[100];
	cin>>buffer;
	char* cmd=strtok(buffer," ");
	if(strcmp(cmd,"login")==0||strcmp(cmd,"register")==0)
	{
		char* username=strtok(NULL," ");
		char* password=strtok(NULL," ");
		this->loginInterface(cmd, username, password);

	}
	else if(strcmp(cmd,"exit")==0)
	{
		endOfSession=true;
	}
	else
	{
		cout<<"wrong command..."<<endl;
	}
	}while(!endOfSession);

	this->exit();





}
void RSPGameClient::loginInterface(char* cmd,char* username,char* password) {
	this->socket=new TCPSocket(SERVER_IP,SERVER_PORT);
	if(socket!=NULL)
	{
		char buffer[100];
		strcat(buffer,cmd);
		strcat(buffer,username);
		strcat(buffer,password);
		int size=htonl(strlen(buffer)+1);
		//send to server <command> <username> <password>
		socket->write((char*)&size, 4);
		socket->write(buffer, size);
		int serverAnswer;
		socket->read((char*)&serverAnswer, 4);
		switch(ntohl(serverAnswer))
		{
		case LOGIN_APPROVED:
		case REGISTER_APPROVED:
			cout<<"connecting to server..."<<endl;
			int newPort;
			socket->read((char*)&newPort, 4);
			sleep(2);
			socket=new TCPSocket(SERVER_IP,ntohl(newPort));
			this->serverinterface(socket);
			break;
		case WRONG_COMMAND_ERROR:
			cout<<"Error:wrong command"<<endl;
			return;
		case WRONG_PASSWORD_ERROR:
			cout<<"Error:password don't match"<<endl;
			return;
		case WRONG_USERNAME_ERROR:
			cout<<"Error:user not exist"<<endl;
			return;
		case USER_EXIST_ERROR:
			cout<<"Error:username already in use"<<endl;
			return;
		}
		endOfSession=false;
		return;




	}
	else
	{
		cout<<"Error: can't connect to game server"<<endl;
		endOfSession=true;
		return;
	}
}





void RSPGameClient::serverinterface(TCPSocket* socket ) {
	bool disconnected=false;
	if(socket!=NULL)
	{
		while(!disconnected)
		{
			cout<<"SERVER MENU:\n"
					"1 show online users\n"
					"2 show high scores\n"
					"3 <username> start game with <username>\n"
					"4 start game with random user\n"
					"5 set user available for random games"
					"6 set user un-available from random games"
					"7 disconnect from server"<<endl;

			char buffer[100];
			cin>>buffer;
			char* cmd=strtok(buffer," ");
			int size=htonl((strlen(buffer)+1));
			switch(cmd)
			{
			case SET_USER_AVAILABLE:
			case SET_USER_UNAVAILABLE:
				socket->write((char*)&size, 4);
				socket->write(buffer, size);
				break;
			case SHOW_USERS:
			case SHOW_HS:
				socket->write((char*)&size, 4);
				socket->write(buffer, size);
				break;
				sleep(2);
				socket->read((char*)&size, 4);
				socket->read(buffer, size);
				cout<<buffer<<endl;
				break;
			case DISCONNECT:
				socket->write((char*)&size, 4);
				socket->write(buffer, size);
				disconnected=true;
				endOfSession=true;
				break;
			case START_GAME_WITH_USER:
			case START_GAME_WITH_RANDOM:

			}
		}
	}
	else
		cout<<"Error:can't connect to server new perted socket"<<endl;
	return;
}

void RSPGameClient::gameInterface() {
}
void RSPGameClient::run() {

}
void RSPGameClient::exit() {
}

RSPGameClient::~RSPGameClient() {
	// TODO Auto-generated destructor stub
}

} /* namespace networkingLab */
