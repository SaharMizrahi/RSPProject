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
#include "MultipleTCPSocketsListener.h"
namespace networkingLab {




RSPGameClient::RSPGameClient(char* u,char* p) {
	sock=new TCPSocket(SERVER_IP,SERVER_PORT);
	this->username=u;
	this->password=p;

}

RSPGameClient::~RSPGameClient() {
	if(sock!=NULL)
	{
		sock->close();
		delete sock;
	}
}
void RSPGameClient::run() {
	if(sock!=NULL)
	{

		bool res=this->loginOrRegister("login");

		if(res==true)
		{
			cout<<"Successfully connected to server"<<endl;
			bool done=false;
			while(!done)
			{
				cout<<"\n*****MENU*****\n1 show online users\n2 show high scores\n3 <username> start game with user\n4 start game with random user\n5 exit\nenter your choice:";
				string str;
				getline(cin,str);
				char buffer[strlen(str.c_str())+1];
				strcpy(buffer,str.c_str());
				char* cmd=strtok(buffer," ");
				if(strcmp(cmd,"1")==0)
				{
					this->showUsers();
				}
				else if(strcmp(cmd,"2")==0)
				{
					this->showHighScores();
				}
				else if(strcmp(cmd,"3")==0)
				{
					char* username=strtok(NULL," ");
				}
				else if(strcmp(cmd,"4")==0)
				{
					this->startGameWith(NULL);
				}
				else if(strcmp(cmd,"5")==0)
				{
					done=true;
				}

			}

		}
		else
		{
			perror("Error: loginOrRegister method(res=false)");
		}

	}
	else
	{
		cout<<"Error:Can't connect to server socket, network error"<<endl;
	}
}

bool RSPGameClient::loginOrRegister(char* cmd) {
	if(this->sock!=NULL)
	{
		if(strcmp(cmd,"login")==0||strcmp(cmd,"register")==0)
		{
			char buffer[100];
			strcat(strcat(strcat(strcat(strcat(buffer,cmd)," "),username)," "),password);
			int size=htonl(strlen(buffer)+1);
			sock->write((char*)&size, 4);
			sock->write(buffer, size);
			sleep(2);
			int answer;
			sock->read((char*)&answer, 4);
			switch(ntohl(answer))
			{
			case LOGIN_APPROVED:
			case REGISTER_APPROVED:
				sleep(2);
				int newPort;
				sock->read((char*)&newPort,4);
				sleep(2);
				//cout<<"new port: "<<ntohl(newPort)<<endl;
				sock=new TCPSocket(SERVER_IP,ntohl(newPort));

				if(sock!=NULL)
				{
					return true;
				}
				else
				{
					perror("new sock  is null( RSPGameClient::loginOrRegister)");
					return false;
				}

			}

		}
		else
		{
			perror("cmd is not login/register( RSPGameClient::loginOrRegister)");
			return false;
		}
	}
	else
	{
		perror("sock  is null( RSPGameClient::loginOrRegister)");
		return false;
	}
}

void RSPGameClient::showHighScores() {
	if(sock!=NULL)
	{
		char buffer[]="2";
		int size=sizeof(buffer);
		sock->write((char*)&size, 4);
		sock->write(buffer, size);
		sleep(2);
		char answer[100];
		int s;
		sock->read((char*)&s, 4);
		sock->read(answer, s);
		if(strcmp(answer,"Error")!=0)
		{
			cout<<answer<<endl;
		}
		else
			cout<<"Error: server problem"<<endl;
	}
	else
	{
		cout<<"Error: can't ask for high scores, socket is closed"<<endl;
	}
}

void RSPGameClient::showUsers() {
	if(sock!=NULL)
	{
		char buffer[]="1";
		int size=(strlen(buffer)+1);
		sock->write((char*)&size, 4);
		sock->write(buffer, size);
		sleep(2);
		char answer[1000];
		int s;
		sock->read((char*)&s, 4);
		sock->read(answer, s);
		if(strcmp(answer,"Error")!=0)
		{
			cout<<answer<<endl;
		}
		else
			cout<<"Error: server problem"<<endl;
	}
	else
	{
		cout<<"Error: can't ask for users list, socket is closed"<<endl;
	}

}

void RSPGameClient::setUserAvailability(bool flag) {

	if(sock!=NULL)
	{
		char* t="5";
		char* f="6";

		int size=htonl((strlen(t)+1));
		sock->write((char*)&size, 4);
		if(flag)
			sock->write(t, size);
		else
			sock->write(f, size);
	}
	else
	{
		cout<<"Error: can't set new availability, socket is closed"<<endl;
	}


}

void RSPGameClient::waitForGame() {
	this->setUserAvailability(true);
	MultipleTCPSocketsListener* listener=new MultipleTCPSocketsListener();
	listener->addSocket(sock);
	TCPSocket* ready=listener->listenToSocket();
	char buffer[10];
	int s;
	ready->read((char*)&s, 4);
	ready->read(buffer, ntohl(s));
	if(strcmp(buffer,"failed")==0)
	{
		cout<<"can't run RSP game"<<endl;
	}
	else if(strcmp(buffer,"start")==0)
	{
		UDPSocket* gameSock=new UDPSocket(SERVER_IP,sock->getPort());
		handleGame(gameSock);
	}




}

void RSPGameClient::handleGame(UDPSocket* gameSocket) {
}

void RSPGameClient::startGameWith(char* username) {
}



} /* namespace networkingLab */
