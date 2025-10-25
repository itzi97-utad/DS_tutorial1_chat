
#include "utils.h"
#include <string>

using namespace std;

class clientManager {
public:
	typedef enum { textMSG, privateMSG, login, logout, ack } msgTypes;

	static inline map<string, int> connectionIds;
	static inline bool isConnectionActive = true;
	static inline bool isClientDisconnected = false;

	static inline vector<unsigned char> chatRemainingMessages;
	static inline mutex messageLock;

	static void resolveClientMessages(int clientId);
};
