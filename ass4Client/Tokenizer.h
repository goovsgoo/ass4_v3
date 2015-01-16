/*
 * Tokenizer.h
 * Created on: 12.1.2014
 *      Author: Goovsgoo
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "connectionHandler.h"
#include "HttpFrame.h"

using HttpClient::ConnectionHandler;
using std::vector;
using std::string;

 namespace HttpP {

class Tokenizer {

private:
	ConnectionHandler* connectionHandler_;
	vector<string> buffer_;

	Tokenizer(const Tokenizer& other);
	Tokenizer operator=(const Tokenizer& other);

public:
	Tokenizer(ConnectionHandler& connectionhandler);
	~Tokenizer();

	HttpFrame* nextFrame();

 };

}

#endif /* TOKENIZER_H_ */
