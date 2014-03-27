#include "proxy.h"  // As an example
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <string>
#include <fstream>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace Proxy;

#include <sstream>
#include <fstream>

template <typename T>
std::string to_string( T Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

int main(int argc, char **argv) 
{
	boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	proxyClient client(protocol);
	transport->open();


	std::ifstream ifs;

	ifs.open ("urls", std::ifstream::in);

	std::string URL;

	std::getline(ifs,URL);

	std::string base = "output_file_";

	int i = 0;

	while (ifs.good())
	{
		std::string page;

		client.getPage(page, URL);

		std::string output = base+to_string(i)+".html";

		++i;

		std::ofstream myfile(output.c_str());
		myfile << page;
		myfile.close();

		std::getline(ifs,URL);
	}

	ifs.close();

	//std::cout << "page: " << std::endl << std::endl;
	//std::cout << page << std::endl;

	//TODO


	transport->close();

	return 0;
}
