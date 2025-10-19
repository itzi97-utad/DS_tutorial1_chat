#include "utils.h"
#include "clientManager.h"
#include <string>
#include <iostream>
#include <string>

using namespace std;

void recvMessagesFromServer(int serverId)
{
	vector<unsigned char> buffer;
	do{
		buffer.clear();
		//receive MSG
		//switch type of MSG
		
		recvMSG(serverId,buffer);
		clientManager::msgTypes type=unpack<clientManager::msgTypes>(buffer);
		
		switch(type){
			case clientManager::ack:
			{ 
				//store ack for the chat
				//lock the shared variable
				clientManager::messageLock.lock();
				pack(clientManager::chatRemainingMessages,clientManager::ack);
				clientManager::messageLock.unlock();
				//unlock
				
			}break;
			case clientManager::textMSG:
			{
				string message;
				//unpack message
				message.resize(unpack<long int>(buffer));
				unpackv(buffer,(char*)message.data(),message.size());
				//print messsage
				cout<<"MESSAGE: "<<message<<"\n";
				//done!
			}break;
			default:
			{
				cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<"unknown message type\n";
			}
		}
		
		//print MSG
	}while(clientManager::isConnectionActive);
	
	clientManager::isClientDisconnected=true;
	
	
}


void chat(int serverId){
	//login
		//ask username
	string userName="";
	vector<unsigned char> buffer;
	cout<<"Write a username:"<<endl;
	getline(cin,userName);
	
		//send login msg
			//pack type
	pack(buffer,clientManager::login);
			//pack username
	pack(buffer,userName.size());
	packv(buffer,userName.data(),userName.size());
		
			//send packet
	sendMSG(serverId,buffer);
			//receive ack
	while(clientManager::chatRemainingMessages.size()==0) usleep(100);
			
	clientManager::messageLock.lock();
	if(unpack<clientManager::msgTypes>(clientManager::chatRemainingMessages)!=clientManager::ack)
			cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<"\n";
	clientManager::messageLock.unlock();
	
	
	string msg="";
	while(msg!="exit"){
	//loop untill "exit"
		//ask a message
		cout<<"Write a message:"<<endl;
		getline(cin,msg);
		//if message== "exit"
		if(msg=="exit")
		{	
			buffer.clear();
			pack(buffer,clientManager::logout);
		}else
		{	
			//send message
			//pack type
			buffer.clear();
			pack(buffer,clientManager::textMSG);
				//pack message
			pack(buffer,msg.size());
			packv(buffer,msg.data(),msg.size());
			
			//recv messages from server
		}
		//send user message
		sendMSG(serverId,buffer);			
		//recv ack
		while(clientManager::chatRemainingMessages.size()==0) usleep(100);

		clientManager::messageLock.lock();
		if(unpack<clientManager::msgTypes>(clientManager::chatRemainingMessages)!=clientManager::ack)
			cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<"\n";
		clientManager::messageLock.unlock();
	}
	
}

int main(int argc,char** argv)
{
	vector<unsigned char> bufferData;
	//connect to the server: 127.0.0.1:1067
	auto serverConnection=initClient("127.0.0.1",1067);

	thread* th=new thread(recvMessagesFromServer,serverConnection.serverId);	
	chat(serverConnection.serverId);
	
	//wait for pending messages:
	while(!clientManager::isClientDisconnected);
	
	closeConnection(serverConnection.serverId);
	//close the server connection
    return 0;
}
