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
using namespace std;

#include <sstream>
#include <fstream>



template <typename T>
std::string to_string( T Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

//client takes host, port and an file of URLs (one URL per line, only URLs, not blank line)
int main(int argc, char **argv) 
{
	if(argc != 4)
	{
		cout << "Mandatory input parameters: host port urls_file" << endl;
		return -1;
	}

	std::string host = argv[1];
	int port = atoi(argv[2]);
	std::string urls_file = argv[3];

	boost::shared_ptr<TSocket> socket(new TSocket(host, port));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	proxyClient client(protocol);
	transport->open();


	std::ifstream myStream;

	myStream.open (urls_file.c_str(), std::ifstream::in);

	std::string URL;

	std::getline(myStream,URL);

	#if DEBUG_MESSAGES == 1

		std::string base = "output_file_";

		int N = 0;
	#endif

	unsigned int totalSize = 0;

	client.resetCounters();//reset the counters so that previous cache initialization is not counted
	
	struct timeval tp;
	gettimeofday( &tp, NULL );

	unsigned long long  begin = 1e6*tp.tv_sec+tp.tv_usec;
	while (myStream.good())
	{
		std::string page;

		client.getPage(page, URL);//RPC call: get the page corresponding to the given URL

		#if DEBUG_MESSAGES == 1

			//write the page as an "output_file_N.html" file
			std::string output = base+to_string(N)+".html";

			std::ofstream myfile(output.c_str());
			myfile << page;
			myfile.close();

			++N;
		#endif

		totalSize += page.size();

		std::getline(myStream,URL);
	}

	gettimeofday( &tp, NULL );
	unsigned long long end = 1e6*tp.tv_sec+tp.tv_usec;

	cout << "Time taken: " << double(end - begin)*1e-6 << " sec" << endl;

	int32_t cached = client.getCachedVolume();
	int32_t not_cached = client.getNetworkVolume();
	int32_t hit = client.getHitCount();
	int32_t miss = client.getMissCount();

	#if DEBUG_MESSAGES == 1
		cout << "Client received " << totalSize << " Bytes from the server" << endl;

		cout << "There were " << hit << " cache hits and " << miss<< " cache misses" << endl;
	#endif

		cout << "Cache hit rate: " << hit/(1.0*hit+miss)*100 << " (% of pages)"<< endl;
		cout << cached << " Bytes came from cache" << endl;
		cout << not_cached << " Bytes came from the web" << endl;

	myStream.close();

	transport->close();

	return 0;
}
