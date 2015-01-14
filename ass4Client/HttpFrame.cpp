/*
 * HttpFrame.cpp
 * Created on: 12.1.2014
 *      Author: Goovsgoo
 */

#include "HttpFrame.h"

using HttpP::HttpFrame;


HttpFrame::HttpFrame(const std::string& status): status_(status), headers_() , body_(""){}

HttpFrame::HttpFrame(const string& status, const headerMap& headers, const string& body): status_(status), headers_(headers) , body_(body){}

HttpFrame::~HttpFrame(){}

string HttpFrame::getStatus() const{
	return status_;
}

string HttpFrame::getHeader(const string& key) const{
	if (headers_.find(key) == headers_.end()){
		return "key does not exist";
	}
	return headers_.at(key);
}

string HttpFrame::getBody() const{
	return body_;
}

string HttpFrame::toString() const{
	std::string frame(status_);
	frame.append("\n");

	for (headerMap::const_iterator i = headers_.begin() ; i != headers_.end() ; i++ ){
		frame.append(i->first).append(":").append(i->second).append("\n");
	}
	frame.append("\n").append(body_).append("\n");

	return frame;
}


