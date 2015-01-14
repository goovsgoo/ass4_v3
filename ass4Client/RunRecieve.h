/*
 * RunRecieve.h
 * Created on: 12.1.2014
 * Author: Goovsgoo
 */

#ifndef RUNRECIEVE_H_
#define RUNRECIEVE_H_

#include "ConnectionHandler.h"
#include "Tokenizer.h"
#include "HttpFrame.h"
#include "RunSend.h"

using namespace HttpP;

namespace HttpClient {

class RunSend;

class RunRecieve {

private:
	RunSend* runSend_;
	ConnectionHandler* connectionHandler_;
	Tokenizer* tokenizer_;
	bool active_;

	RunRecieve(const RunRecieve& other);
	RunRecieve operator=(const RunRecieve& other);

	void analyzeFrame(const HttpFrame& frame);

public:
	RunRecieve(RunSend& RunSend, ConnectionHandler& connectionHandler);
	~RunRecieve();

	void run();
	void close();
};


}


#endif /* RUNRECIEVE_H_ */
