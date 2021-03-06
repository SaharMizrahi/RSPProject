/*
 * RSPGameClient.cpp
 *
 *  Created on: Aug 15, 2017
 *      Author: user
 */

#include "RSPGameClient.h"
#include <string.h>
#include "Protocol.h"
#include "MultipleTCPSocketsListener.h"
#include "UDPSocket.h"

#include "Protocol.h"
using namespace std;
namespace networkingLab {

RSPGameClient::RSPGameClient(char* u,char* p,char* c) {
	sock=new TCPSocket(SERVER_IP,SERVER_PORT);
	this->username=u;
	this->password=p;
	this->cmd=c;

}

RSPGameClient::~RSPGameClient() {
	if(sock!=NULL)
	{
		sock->close();
		delete sock;
	}
}


bool RSPGameClient::loginOrRegister() {
	if(this->sock!=NULL)
	{
		if(strcmp(cmd,"login")==0||strcmp(cmd,"register")==0)
		{
			char buffer[100];
			memset(buffer,0,100);
			strcat(strcat(strcat(strcat(strcat(buffer,cmd)," "),username)," "),password);
			int size=strlen(buffer);
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

		int size=sizeof(t);
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

/*void RSPGameClient::waitForGame() {
	this->setUserAvailability(true);
	char buffer[10];
	int s;
	sleep(1);
	int otherPort;
	sock->read((char*)&otherPort, 4);
	if(otherPort!=-1)
	{
		handleGame(otherPort);

	}







}*/

/*int RSPGameClient::sendToOtherUser(UDPSocket* Sock, int otherPort) {
	cout<<"Choose one: r-rock s-scissors p-paper b-exit undefined choice will cause automatic choosing rock"<<endl;
	cout<<this->username<<": ";
	char buf[10];
	memset(buf,0,10);
	cin>>buf;

	if(strcmp(buf,"s")==0||strcmp(buf,"scissors")==0)
	{
		char*msg="s";
		Sock->sendTo(msg, SERVER_IP, otherPort);

		return SCISSORS;
	}
	else if(strcmp(buf,"p")==0||strcmp(buf,"paper")==0)
	{
		char*msg="p";
		Sock->sendTo(msg, SERVER_IP, otherPort);
		return PAPER;
	}
	else if(strcmp(buf,"b")==0||strcmp(buf,"exit")==0)
	{
		char*msg="b";
		Sock->sendTo(msg, SERVER_IP, otherPort);
		return BYEBYE;
	}
	else//rock
	{
		char*msg="r";
		Sock->sendTo(msg, SERVER_IP, otherPort);
		return ROCK;
	}


}*/

int RSPGameClient::recvFromOtherUser(UDPSocket* mySock) {
	char buf[10];
	memset(buf,0,10);
	mySock->recv(buf, 10);
	if(strcmp(buf,"r")==0)
		return ROCK;
	else if(strcmp(buf,"p")==0)
		return PAPER;
	else if(strcmp(buf,"s")==0)
		return SCISSORS;
	else if(strcmp(buf,"b")==0)
		return BYEBYE;
	else
	{
		//randomly return rock
		return ROCK;
	}


}

int RSPGameClient::policy(int c1, int c2)
{
	cout<<"*********"<<endl;
	if(c1==c2)
	{
		cout<<this->username<<" draw"<<endl;
		cout<<"*********"<<endl;

		return 0;
	}
	switch(c1)
	{
	case ROCK:
		if(c2==PAPER)
		{
			cout<<this->username<<" lose :-("<<endl;
			cout<<"*********"<<endl;

			return -1;
		}
		else if(c2==SCISSORS)
		{
			cout<<this->username<<" win :-)"<<endl;
			cout<<"*********"<<endl;


			return 1;
		}
	case PAPER:
		if(c2==ROCK)
		{
			cout<<this->username<<" win :-)"<<endl;
			cout<<"*********"<<endl;

			return 1;
		}
		else if(c2==SCISSORS)
		{
			cout<<this->username<<" lose :-("<<endl;
			cout<<"*********"<<endl;

			return -1;
		}
	case SCISSORS:
		if(c2==PAPER)
		{
			cout<<this->username<<" win :-)"<<endl;
			cout<<"*********"<<endl;

			return 1;
		}
		else if(c2==ROCK)
		{
			cout<<this->username<<" lose :-("<<endl;
			cout<<"*********"<<endl;

			return -1;
		}
	default:
		return 0;
	}

}

/*void RSPGameClient::handleGame(int otherPort) {
	UDPSocket* mySock=new UDPSocket(this->sock->getPort());
	int c1,c2;
	int res=0;
	if(mySock!=NULL)
	{


			if(mySock->getPort()>otherPort)
			{

				c1=sendToOtherUser(mySock, otherPort);
				c2=recvFromOtherUser(mySock);
			}
			else
			{
				c2=recvFromOtherUser(mySock);
				c1=sendToOtherUser(mySock, otherPort);

			}
			if(c1==BYEBYE||c2==BYEBYE)//finish game
			{
				if(c2==BYEBYE)
					cout<<"other player quit the game..game ends!"<<endl;
			}
			else
			{
				res+=policy(c1, c2);
				cout<<username<<": "<<res<<endl;

			}







			char* result;
		if(res==0)
		{
			result="8 draw";
		}
		else if(res>0)
		{
			result="8 win";

		}
		else if(res<0)
		{
			result="8 lose";

		}
		int size=strlen(result);
		this->sock->write((char*)&size, 4);
		this->sock->write(result, size);



	}



}*/

int RSPGameClient::startGameWith(char* username) {
	if(username!=NULL)
	{
		char msg[100];
		memset(msg,0,100);
		strcat(msg,"3 ");
		strcat(msg,username);
		int size=sizeof(msg);
		sock->write((char*)&size, 4);
		sock->write(msg, size);


	}
	else
	{
		char* msg="4";
		int s=sizeof(msg);
		sock->write((char*)&s, 4);
		sock->write(msg, s);
	}

	sleep(1);
	int otherPort;
	sock->read((char*)&otherPort, 4);
	return otherPort;





}

void RSPGameClient::disconnectFromServer() {

	char* msg="7";
	int s=sizeof(msg);
	this->sock->write((char*)&s, 4);
	this->sock->write(msg, s);
	sleep(1);
	this->sock->close();
}



} /* namespace networkingLab */


