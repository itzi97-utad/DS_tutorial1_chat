#include "clientManager.h"

void clientManager::resolveClientMessages(int clientId) {

	string userName = "";
	vector<unsigned char> buffer;
	bool logOut = false;
	do {
		// receive a packet from client
		recvMSG(clientId, buffer);
		msgTypes type = unpack<msgTypes>(buffer);
		// switch type of the packet
		switch (type) {
			// type login
		case login: { // unpack username
			userName.resize(unpack<long int>(buffer));
			unpackv(buffer, (char *)userName.data(), userName.size());
			// add username to client list
			connectionIds[userName] = clientId;
		} break;
			// search by username
			// type textMSG
		case textMSG: {
			// º MSG
			string msg;
			msg.resize(unpack<long int>(buffer));
			unpackv(buffer, (char *)msg.data(), msg.size());
			// broadcast MSG
			msg = userName + " says: " + msg;
			// create textMSG packet
			buffer.clear();
			pack(buffer, textMSG);
			pack(buffer, msg.size());
			packv(buffer, msg.data(), msg.size());
			for (auto client : connectionIds) {
				if (client.first != userName) {
					sendMSG(client.second, buffer);
				}
			}
		} break;
		case privateMSG: {
			// º recipient
			string recipient;
			recipient.resize(unpack<long int>(buffer));
			unpackv(buffer, (char *)recipient.data(), recipient.size());

			// º MSG
			string msg;
			msg.resize(unpack<long int>(buffer));
			unpackv(buffer, (char *)msg.data(), msg.size());

			// create privateMSG packet
			buffer.clear();
			pack(buffer, privateMSG);

			// pack recipient
			pack(buffer, recipient.size());
			packv(buffer, recipient.data(), recipient.size());
			// pack message
			pack(buffer, msg.size());
			packv(buffer, msg.data(), msg.size());
			auto recipientIt = connectionIds.find(recipient);
			if (recipientIt != connectionIds.end()) {
				sendMSG(recipientIt->second, buffer);
			}
		} break;

		// type logout
		case logout: {
			// delete the userName
			connectionIds.erase(userName);
			// activate logout protocol
			logOut = true;
			// send a "userName left the site" message
			buffer.clear();
			string message = userName + " left the site";
			pack(buffer, textMSG);        // pack the type
			pack(buffer, message.size()); // pack the message
			packv(buffer, message.data(), message.size());
			for (auto client : connectionIds) { // broadcast message
				sendMSG(client.second, buffer);
			}
		} break;
		}
		buffer.clear();
		pack(buffer, ack);
		sendMSG(clientId, buffer);
	} while (!logOut);
	// close connection

	closeConnection(clientId);
}
