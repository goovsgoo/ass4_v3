/*
 * HttpFrame.h
 * Created on: 12.1.2014
 *      Author: Goovsgoo
 */

#ifndef HTTPFRAME_H_
#define HTTPFRAME_H_

#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <string.h>


using std::string;
using std::vector;


namespace HttpP {

typedef std::map<string,string> headerMap;

class HttpFrame{

protected:
	const string status_;
	headerMap headers_;
	string body_;

public:
	HttpFrame(const string& status);
	HttpFrame(const string& status, const headerMap& headers, const string& body);
	virtual ~HttpFrame();

	string getStatus() const;
	string getHeader(const string& key) const;
	string getBody() const;

	string toString() const;

};

/*
 * Client Frames
 */
class LoginFrame: public  HttpFrame {
public:
	LoginFrame(const std::string& host, const std::string& username, const std::string& phone);
};

class LogoutFrame: public HttpFrame {
public:
	LogoutFrame(const int receipt);
};

class SendFrame: public HttpFrame {
public:
	SendFrame(const std::string& topic, const std::string& body);
};

class SubscribeFrame: public HttpFrame {
public:
	SubscribeFrame(const std::string& topic, const int id);
};

class UnsubscribeFrame: public HttpFrame {
public:
	UnsubscribeFrame(const int id);
};


/*
*input Frames from the server
 */
class ParshFrame: public HttpFrame {

public:
	static HttpFrame* generate(const vector<string>& input) {
				unsigned int i = 0;
				while (i < input.size() && input.at(i) == "") {  
					i++;
				}
				string status =  input.at(i);
				i++;

				headerMap headers;
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

};


}

#endif /* HTTPFRAME_H_ */
