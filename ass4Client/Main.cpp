#include "RunSend.h"
#include <boost/thread.hpp>
#include <boost/locale.hpp>

int main (int argc,char *argv[]) {

	if (argc < 2) {
        cerr << "Server ip address and port - unvalid" << endl;
        return -1;
    }
    string host = argv[0];
	short port = (short) strtoul(argv[1], NULL, 0);

	HttpClient::RunSend* client = new HttpClient::RunSend(host,port);

	boost::thread threadRunSend(&*client);
	threadRunSend.join();

	cout << "*Start RunSend thread*" << endl;

    return 0;
}
