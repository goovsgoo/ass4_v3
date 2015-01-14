#include "RunSend.h"
#include <boost/thread.hpp>
#include <boost/locale.hpp>

int main (int argc,char *argv[]) {

	HttpClient::RunSend* client = new HttpClient::RunSend(argv[0],argv[1]);

	boost::thread threadRunSend(&HttpClient::RunSend::run, &*client);
	threadRunSend.join();

	cout << "***" << endl;

    return 0;
}
