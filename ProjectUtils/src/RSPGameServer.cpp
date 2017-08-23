/*
 * RSPGameServer.cpp
 *
 *  Created on: Aug 17, 2017
 *      Author: user
 */

#include "RSPGameServer.h"
#include <string.h>
#include "fstream"
#include "Protocol.h"
namespace networkingLab {


void RSPGameServer::run() {
	clientHandler->start();
	stopLogin=false;
	TCPSocket* ready=NULL;
	while((ready=bindingSocket->listenAndAccept())!=NULL&&!stopLogin)
	{
		stopLogin=true;
		if(ready!=NULL)
		{
			bool res=handleConnectedClient(ready);
			if(res==false)
			{

				perror("Error: can't add new client");
			}
			ready=NULL;
		}

		stopLogin=false;
	}
}
string RSPGameServer::encryptDecrypt(string toEncrypt) {
    char key = 'K'; //Any char will work
    string output = toEncrypt;

    for (int i = 0; i < toEncrypt.size(); i++)
        output[i] = toEncrypt[i] ^ key;

    return output;
}

RSPGameServer::RSPGameServer(int port) {

	this->bindingSocket=new TCPSocket(port);
	this->clientHandler=new RSPClientHandler(&this->socketsVector,&this->usersVector);
	this->stopLogin=true;

}

int RSPGameServer::authenticateClient(char* cmd, char* username,char* password) {
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
					char* str=new char[line.length()];
					strcpy(str,s);
					char* u=strtok(str," ");
					char* p=strtok(NULL," ");
					if(strcmp(u,username)==0)
					{
						if(strcmp(p,encryptDecrypt(password).c_str())==0)//username exist,password match to username
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
					strcat(buffer,encryptDecrypt(password).c_str());
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
		perror("cant open");
		return -2;

}
bool RSPGameServer::isUserAlreadyLogin(char* username) {

	for(unsigned int i=0;i<this->usersVector.size();i++)
	{
		if(strcmp(usersVector.at(i)->getUsername(),username)==0)
		{
			return true;
		}
	}
	return false;

}
bool RSPGameServer::handleConnectedClient(TCPSocket* sock) {
	char buffer[100];
	memset(buffer,0,100);
	int size;
	sock->read((char*)&size, 4);
	sock->read(buffer, size);
	char* cmd=strtok(buffer," ");
	if(strcmp(cmd,"login")==0||strcmp(cmd,"register")==0)
	{
		char* username=strtok(NULL," ");
		char* password=strtok(NULL," ");
		if(!isUserAlreadyLogin(username))
		{
			int res=authenticateClient(cmd, username, password);
			switch(res)
			{
			case LOGIN_APPROVED:
			case REGISTER_APPROVED:
				sock->write((char*)&res, 4);
				this->addNewClient(sock, username, sock->getPort());
				return true;
			case OPEN_FILE_ERROR:
			case WRONG_COMMAND_ERROR:
			case WRONG_PASSWORD_ERROR:
			case WRONG_USERNAME_ERROR:
			case USER_EXIST_ERROR:
				sock->write((char*)&res, 4);
				sleep(2);
				return false;
			}
		}
		else
		{
			int r=USER_ALREADY_LOGIN;
			sock->write((char*)&r, 4);
			sleep(2);
			return false;
		}
	}

	else
	{
		return false;
	}
}

void RSPGameServer::addNewClient(TCPSocket* sock, char* username, int port) {
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
	int random=rand()%100+1;
	int newPort=random+sock->getPort();
	int net=htonl(newPort);
	sock->write((char*)&net, 4);
	sleep(1);
	sock=new TCPSocket(newPort);
	char * u;
	u=new char[strlen(username)+1];
	strcpy(u,username);
	RSPUser* newUser=new RSPUser(u,rank,sock->listenAndAccept(),newPort);
	this->socketsVector.push_back(newUser->getSocket());
	this->clientHandler->update(socketsVector);
	this->usersVector.push_back(newUser);
	this->clientHandler->update(&usersVector);

}

RSPGameServer::~RSPGameServer() {
	// TODO Auto-generated destructor stub
}

} /* namespace networkingLab */


