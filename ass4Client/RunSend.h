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
		string host_;
		short port_;
		string Cookie_;


		RunSend(const RunSend&);
		RunSend operator=(const RunSend&);

		void RunSend::FirstConnection();
		string convertCommand(string& input);
		string login(istringstream& iss);
		string logout();
		//string getTime();

		string list(istringstream& iss);
		string CreateGroup(istringstream& iss);
		string send(istringstream& iss);
		string addRemove(istringstream& iss,string action);
		string queue(istringstream& iss);
		string exit();
		

	public:

		RunSend();
		RunSend(string host,short port);
		~RunSend();
		void operator()();
		void disconnect();
		void saveCookie(string Cookie);

	};
}
#endif /* RUNSEND_H_ */
