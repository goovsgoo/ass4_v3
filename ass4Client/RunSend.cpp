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
				if (!connectionHandler_->sendFrameAscii(command, '\0')) {
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

void RunSend::connect() {
	isConnected_ = true;
	idMap_[user_] = 0;
	cout << "Connected Successfully!" << endl;
	logger_.append("<HTML>\n<HEAD>\n<h1>").append(user_).append("</h1>\n").append(getTime())
			.append("\n</HEAD>\n<BODY>\n<PRE>\n").append("<table width=\"100%\" border>\n")
			.append("<tr>").append("<th>Time</th>").append("<th>User</th>").append( "<th>Message</th>")
			.append("</tr>\n");
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
	} else 
	if (commandName == "Exit"){
		http = stop();
	} else 
	if (commandName == "List Users"){
		http = clients(iss);
	} else {
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
	int phone;
	iss >> phone;

	LoginFrame frame(username, phone);

	return frame.toString();
}

string RunSend::logout(){
	cout << "Disconnecting..." << endl;

	receipt_++;
	DisconnectFrame frame(receipt_);
	return frame.toString();
}


string RunSend::follow(istringstream& iss){
	string username;
	iss >> username;

	nextId_++;
	SubscribeFrame frame(username, nextId_);
	if (idMap_.find(username) == idMap_.end()){
		idMap_[username] = nextId_;
	}
	return frame.toString();
}


string RunSend::unfollow(istringstream& iss){
	string username;
	iss >> username;
	UnsubscribeFrame frame(idMap_[username]);
	idMap_.erase(username);
	return frame.toString();
}

string RunSend::tweet(istringstream& iss){
	string message(iss.str().substr(6, iss.str().size()-6));
	SendFrame frame(user_, message);
	return frame.toString();
}

string RunSend::stop(){
	SendFrame frame("server", "stop");
	return frame.toString();
}

string RunSend::clients(istringstream& iss){
	string param;
	iss >> param;

	SendFrame frame("server", "clients\n"+param);
	return frame.toString();
}

string RunSend::stats(){
	SendFrame frame("server", "stats");
	return frame.toString();
}

int RunSend::fixNextId() {
	nextId_--;
	return nextId_;
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


