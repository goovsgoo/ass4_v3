#ifndef RUNSEND_H_
#define RUNSEND_H_

#include <string>
#include <map>
#include "ConnectionHandler.h"
#include "RunRecieve.h"
#include "HttpFrame.h"

using namespace std;

namespace HttpClient {

class RunRecieve;

class RunSend {
private:
		ConnectionHandler* connectionHandler_;
		RunRecieve* runRecieve_;
		bool isConnected_;
		bool isActive_;
		string user_;
		int receipt_;
		string logger_;
		int nextId_;
		map<string,int> idMap_;
		string host_;
		short port_;
		string Cookie_;


		RunSend(const RunSend&);
		RunSend operator=(const RunSend&);

		void RunSend::FirstConnection();
		string convertCommand(string& input);
		string login(istringstream& iss);
		string logout();
		string follow(istringstream& iss);
		string unfollow(istringstream& iss);
		string tweet(istringstream& iss);
		string stop();
		string clients(istringstream& iss);
		string stats();
		string getTime();
		void printLog();
		

	public:

		RunSend();
		RunSend(string host,short port);
		~RunSend();
		void run();
		void RunSend::run(string host,short port)

		void connect();
		void disconnect(int receipt);
		void disconnect();
		int fixNextId();
		void log(HttpP::HttpFrame frame);
		void saveCookie(const HttpFrame& frame);

	};
}
#endif /* RUNSEND_H_ */
