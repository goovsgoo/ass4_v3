#ifndef CONNECTION_HANDLER__
#define CONNECTION_HANDLER__
                                           
#include <string>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace HttpClient {
	
	class ConnectionHandler {
	private:
		boost::asio::io_service io_service_;   // Provides I/O function
		tcp::socket socket_;

	public:
		ConnectionHandler();
		virtual ~ConnectionHandler();

		bool isActive();

		// Connect to remote server
		bool connect(std::string host, short port);

		// Read fixed number for block
		// @Returns false if the connection is closed before bytesToRead bytes can be read
		bool getBytes(char bytes[], unsigned int bytesToRead);
	
		// Send fixed number for block
		// @Returns false if the connection is closed before all data was sent
		bool sendBytes(const char bytes[], int bytesToWrite);

		// Read ascii line from remote server
		// @Returns false if the connection closed before newline can be read
		bool getLine(std::string& line);

		// Send ascii line from remote server
		// Returns false if the connection closed before all data was sent
		bool sendLine(std::string& line);

		// Get Ascii data from remote server until "End Character"
		// @Returns false if the connection closed before delimiter character
		bool getFrameAscii(std::string& frame, char delimiter);

		// Send message to remote host
		// Returns false in case connection is closed before all the data is sent.
		bool sendFrameAscii(const std::string& frame, char delimiter);

		// Close down the connection
		void close();

	}; //class ConnectionHandler
}
#endif
