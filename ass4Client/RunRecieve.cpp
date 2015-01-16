#include "RunRecieve.h"

using HttpClient::RunRecieve;
using namespace HttpP;
using namespace HttpClient;


RunRecieve::RunRecieve(RunSend& runSend, ConnectionHandler& connectionHandler): runSend_(&runSend), connectionHandler_(&connectionHandler), tokenizer_(), active_(false){
	tokenizer_ = new Tokenizer(*connectionHandler_);
}

RunRecieve::~RunRecieve(){
	delete connectionHandler_;
	delete tokenizer_;
}

void RunRecieve::run(){
	active_ = true;

	while (active_) {
		try {
			HttpFrame* frame = tokenizer_->nextFrame();
			analyzeFrame(*frame);
			delete frame;
		} catch (std::exception& e) {
			std::cerr << "Lost connection to server." << std::endl;
			runSend_->disconnect();
		}

	}

}

void RunRecieve::analyzeFrame(const HttpFrame& frame){
	
	string curStatus = frame.getStatus();

	if (curStatus=="HTTP/1.1 403"||curStatus=="HTTP/1.1 404"||curStatus=="HTTP/1.1 405"||curStatus=="HTTP/1.1 418") {
			std::cout << "server: " << frame.toString() << std::endl;
	}
	else if (frame.getStatus() == "HTTP/1.1 200") {
		string Cookie = frame.getHeader("set-Cookie");
		runSend_->RunSend::saveCookie(Cookie);
		std::cout << "server: " << frame.getBody() << std::endl;
	}
	else{
		std::cout << "The data from server is not vaild - " << frame.toString() << std::endl;
	}
}

void RunRecieve::close(){
	active_ = false;
	delete this;
}
