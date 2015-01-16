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

	static HttpFrame* generate(const vector<string>& input);

};
}

#endif /* HTTPFRAME_H_ */
