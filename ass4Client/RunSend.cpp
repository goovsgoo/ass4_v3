#include "RunSend.h"
#include "HttpFrame.h"

#include <boost/thread.hpp>
#include <boost/locale.hpp>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <fstream>

using namespace HttpClient;
using namespace HttpP;

RunSend::RunSend(): connectionHandler_() , runRecieve_(), isConnected_(false) , isActive_(true), user_(), receipt_(0), logger_(""), nextId_(0), idMap_(){}
RunSend::RunSend(string host,short port):Cookie_(), host_(host) , port_(port), connectionHandler_() , runRecieve_(), isConnected_(false) , isActive_(true), user_(), receipt_(0), logger_(""), nextId_(0), idMap_(){}

RunSend::~RunSend(){}

void RunSend::run(){
	RunSend::FirstConnection();

	runRecieve_ = new RunRecieve(*this, *connectionHandler_);
	boost::thread threadRunRecieve(&RunRecieve::run, runRecieve_);

	std::cout << "the client now active. Please login:" << std::endl;
	while (isActive_) {
		string input;
		getline(cin, input);
		if (input.size() > 0) {
			string command = convertCommand(input);

			if (input == "exit_client"){
				isActive_ = false;
			}
			else if (command.size() > 0){
				if (!connectionHandler_->sendFrameAscii(command, '$')) {
						std::cout << "Not Connected.\n" << std::endl;
				}
			}
		}

	}

	if (isConnected_) {
		connectionHandler_->sendFrameAscii(logout(), '\0');
	}

	// wait for full disconnection.
	while (isConnected_) {}
	delete this;

}



void RunSend::disconnect(int receipt){
	if (receipt == receipt_){
		printLog();
		user_ = "";
		cout << "Disconnected successfully." << endl;
		isConnected_ = false;
		runRecieve_->close();
	}
}

void RunSend::disconnect(){
	printLog();
	user_ = "";
	isConnected_ = false;
	runRecieve_->close();
}

string RunSend::convertCommand(string& input){
	
	string http("");
	istringstream iss(input);
	string commandName;
	iss >> commandName;

	if (commandName == "Login"){
		http = login(iss);
	} else 
	if (commandName == "Logout"){
		http = logout();
	}else 
	if (commandName == "List"){
		iss >> commandName;
		if (commandName == "Users"){
			http = listUsers();
		}else 
		if (commandName == "Groups"){
			http = listGroups();
		}
		else 
		if (commandName == "Group"){
			http = listGroup(iss);
		}
		else {
		//unvalid order
		http = iss.str().append("\n");
		}
	}else 
	if (commandName == "Send"){
		iss >> commandName;
		if (commandName == "User"){
			http = sendUser(iss);
		}else 
		if (commandName == "Group"){
			http = sendGroup(iss);
		}
		else {
		//unvalid order
		http = iss.str().append("\n");
		}
	}else 
	if (commandName == "Add"){
		http = add(iss);
	} else 
	if (commandName == "Remove"){
		http = remove(iss);
	} else 
	if (commandName == "Exit"){
		http = exit();
	} else {
		//unvalid order
		http = iss.str().append("\n");
	}

	return http;
}

void RunSend::saveCookie(const HttpFrame& frame){
	Cookie_=frame.getHeader("set-Cookie");
}

void RunSend::FirstConnection(){
	connectionHandler_ = new ConnectionHandler();
	if (!connectionHandler_->connect(host_, port_)) {
		std::cerr << "Cannot connect to " << host_ << ":" << port_ << std::endl;
	}
}

string RunSend::login(istringstream& iss){
	string username;
	iss >> username;
	string phone;
	iss >> phone;
	
	std::string frame;
	frame.append("POST /login.jps HTTP/1.1").append("/n");
	frame.append("/n");
	frame.append("Username=").append(username).append("&").append("Phone=").append(phone).append("/n");
	frame.append("$");

	return frame;
}

string RunSend::logout(){
	cout << "Disconnecting..." << endl;

	std::string frame;
	frame.append("GET /logout.jps HTTP/1.1").append("/n");
	frame.append("Cookie: ").append(Cookie_);
	frame.append("/n");
	frame.append("$");

	return frame;
}


string RunSend::listUsers(){
	cout << "listUsers.../n" << endl;

	std::string frame;
	frame.append("GET /list.jps HTTP/1.1").append("/n");
	frame.append("Cookie: ").append(Cookie_);
	frame.append("/n");
	///////////////////////// need type
	frame.append("$");

	return frame;
}
string RunSend::listGroups(){
}
string RunSend::listGroup(istringstream& iss){}
string RunSend::sendUser(istringstream& iss){}
string RunSend::sendGroup(istringstream& iss){}
string RunSend::add(istringstream& iss){}
string RunSend::remove(istringstream& iss){}
string RunSend::exit(){}

		
		
		
		
		/*
string RunSend::tweet(istringstream& iss){
	string message(iss.str().substr(6, iss.str().size()-6));
	SendFrame frame(user_, message);
	return frame.toString();
}




string RunSend::getTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}

void RunSend::log(StompFrame frame) {
	string sender = frame.getHeader("destination").substr(7, frame.getHeader("destination").size()-7);
	if (sender != user_) {
		logger_.append("<tr>").append("<td>")
			.append(getTime())
			.append("</td>").append("<td>")
			.append(sender).append("</td><td>").append(frame.getBody())
	    	.append("</td>").append("</tr>\n");
	}
}

void RunSend::printLog() {
	logger_.append("</table>\n  </PRE></BODY>\n</HTML>\n");
	string filename("./logs/");
	filename.append(user_).append("_").append(getTime()).append(".html");
	ofstream logFile(filename.c_str());
	if (!logFile.is_open()) {
		system("mkdir logs");
		logFile.open(filename.c_str());
	}
	logFile << logger_;
	logger_.clear();
}


*/