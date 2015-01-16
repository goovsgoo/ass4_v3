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

RunSend::RunSend(): connectionHandler_() , runRecieve_(), isConnected_(false) , isActive_(true){}
RunSend::RunSend(string host,short port):Cookie_(), host_(host) , port_(port), connectionHandler_() , runRecieve_(), isConnected_(false) , isActive_(true){}

RunSend::~RunSend(){}

void RunSend::operator()(){
	RunSend::FirstConnection();

	runRecieve_ = new RunRecieve(*this, *connectionHandler_);
	boost::thread threadRunRecieve(&RunRecieve::run, runRecieve_);

	std::cout << "the client now active. Please login:" << std::endl;
	while (isActive_) {
		string input;
		getline(cin, input);
		if (input.size() > 0) {
			string command = convertCommand(input);

			if (command == "exit_client"){
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


void RunSend::disconnect(){
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
		http = list(iss);
	}else 
	if (commandName == "Create"){
		http = CreateGroup(iss);
	}else 
	if (commandName == "Send"){
		iss >> commandName;
		http = send(iss);
	}else 
	if (commandName == "Add"){
		http = addRemove(iss,"add_user.jsp");
	} else 
	if (commandName == "Remove"){
		http = addRemove(iss,"remove_user.jsp");
	} else 
	if (commandName == "Exit"){
		http = exit();
	} else {
		//unvalid order
		//http = iss.str().append("\n");
		cout << "Invalid command : "<< iss.str().append("\n") << endl;
		commandName="";
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
	frame.append("POST /login.jsp HTTP/1.1").append("/n");
	frame.append("/n");
	frame.append("Username=").append(username).append("&").append("Phone=").append(phone).append("/n");
	frame.append("$");

	return frame;
}

string RunSend::logout(){
	cout << "Disconnecting..." << endl;

	std::string frame;
	frame.append("GET /logout.jsp HTTP/1.1").append("/n");
	frame.append("Cookie: ").append(Cookie_).append("/n");
	frame.append("/n");
	frame.append("$");

	return frame;
}

string RunSend::list(istringstream& iss){
	cout << "list.../n" << endl;
	string groupName="";
	string type;
	iss >> type;

	std::string frame;
	frame.append("GET /list.jsp HTTP/1.1").append("/n");
	frame.append("Cookie: ").append(Cookie_).append("/n");
	frame.append("List:").append(type);		
	int size = iss.tellg();
	if(size!=0){
		iss >> groupName;
		frame.append(" ").append(groupName);
	}
	frame.append("/n");
	frame.append("$");

	return frame;
}

string RunSend::CreateGroup(istringstream& iss){
	string groupName;
	string userNameI;
	iss >> groupName;	//for the "group" word
	iss >> groupName;
	std::string frame;

	frame.append("POST /create_group.jsp HTTP/1.1").append("/n");
	frame.append("Cookie: ").append(Cookie_).append("/n");
	frame.append("GroupName:").append(groupName).append("/n");
	frame.append("Users:");
	int size = iss.tellg();
	while(size!=0){
		iss >> userNameI;
		frame.append(userNameI).append(",");
		size = iss.tellg();
	}
	frame.pop_back();  //erase the last ","
	frame.append("/n");
	frame.append("/n");
	frame.append("$");

	return frame;
}

string RunSend::send(istringstream& iss){
	string type;
	string target;
	string msg;
	iss >> type;
	iss >> target;
	std::string frame;

	frame.append("POST /send.jsp HTTP/1.1").append("/n");
	frame.append("Cookie: ").append(Cookie_).append("/n");
	frame.append("Type:").append(type).append("/n");
	frame.append("MsgTarget:").append(target).append("/n");
	frame.append("Content:");
	int size = iss.tellg();
	while(size!=0){
		iss >> msg;
		frame.append(msg);
		size = iss.tellg();
	}
	frame.append("/n");
	frame.append("/n");
	frame.append("$");

	return frame;

}

string RunSend::addRemove(istringstream& iss,string action){
	string groupName;
	string phoneNum;
	iss >> groupName;
	iss >> phoneNum;
	std::string frame;

	frame.append("POST /").append(action).append(" HTTP/1.1").append("/n");
	frame.append("Cookie: ").append(Cookie_).append("/n");
	frame.append("Target:").append(groupName).append("/n");
	frame.append("User:").append(phoneNum).append("/n");
	frame.append("/n");
	frame.append("$");

	return frame;


}

string RunSend::queue(istringstream& iss){

	std::string frame;

	frame.append("POST /queue.jsp HTTP/1.1").append("/n");
	frame.append("Cookie: ").append(Cookie_).append("/n");
	frame.append("/n");
	frame.append("$");

	return frame;


}
string RunSend::exit(){
	cout << "Currently empty action.../n" << endl;
	logout();										//not sure???
	return "exit_client";
}

		
	
/*
string RunSend::getTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}
*/