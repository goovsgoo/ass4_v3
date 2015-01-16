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

static HttpFrame* generate(const vector<string>& input) {
				unsigned int i = 0;
				while (i < input.size() && input.at(i) == "") {  
					i++;
				}
				string status =  input.at(i);
				i++;

				std::map<string,string> headers;
				// parse headers while line is not empty.
				while (i < input.size() && input.at(i) != "") {
					int delimeter = input.at(i).find(':');
					string key = input.at(i).substr(0, delimeter);
					string value = input.at(i).substr(delimeter+1, input.at(i).size()-delimeter-1);
					headers[key] = value;
					i++;
				}
				i++;

				string body("");
				// parse body until end of frame.
				while (i < input.size()) {
					if (input.at(i) != ""){
						if (body.size() > 0){
							body.append("\n");
						}
						body.append(input.at(i));
					}
					i++;
				}
				HttpFrame* frame = new HttpFrame(status, headers, body);

				return frame;
			}