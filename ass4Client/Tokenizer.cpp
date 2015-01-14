/*
 * Tokenizer.cpp
 *  Created on: 12.1.2014
 *      Author: Goovsgoo
 */

#include "Tokenizer.h"
using namespace HttpP;

Tokenizer::Tokenizer(ConnectionHandler& connectionhandler): connectionHandler_(&connectionhandler), buffer_(){}
Tokenizer::~Tokenizer(){}

HttpFrame* Tokenizer::nextFrame(){
	vector<string> frame;
	frame.push_back("");
	int lineIndx = 0;

	char ch;
	    try {
			do{
				connectionHandler_->getBytes(&ch, 1);
				if (ch == '\n') {
					frame.push_back("");
					lineIndx++;
				}
				else {
					frame.at(lineIndx).append(1, ch);
				}
	        }while (ch != '$');

	    } catch (std::exception& e) {
	    	throw;
	    }


	return ParshFrame::generate(frame);
}

