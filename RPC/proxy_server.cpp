// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "proxy.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <curl/curl.h>

#include <iostream>

#include "NoCache.h"
#include "FIFOCache.h"
#include "RandomCache.h"
#include "LRUCache.h"
#include "LFUCache.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace ::Proxy;

using namespace std;

struct wd_in
{
	size_t size;
	size_t len;
	char *data;
};

/* This function is registered as a callback with CURL.  As the data
 from the requested webpage is returned in chunks, write_data is
 called with each chunk.  */
static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	struct wd_in *wdi = (struct wd_in *)userp;

	while (wdi->len + (size * nmemb) >= wdi->size)
	{
		/* check for realloc failing in real code. */
		wdi->data = (char*)realloc(wdi->data, wdi->size * 2);
		wdi->size *= 2;
	}

	memcpy(wdi->data + wdi->len, buffer, size * nmemb);
	wdi->len += size * nmemb;

	return size * nmemb;
}

class proxyHandler: virtual public proxyIf
{
private:
	string* loadPage(const string& URL)
	{
		CURL *curl;
		CURLcode res;
		struct wd_in wdi;

		memset(&wdi, 0, sizeof(wdi));

		/* Get a curl handle.  Each thread will need a unique handle. */
		curl = curl_easy_init();

		if (NULL != curl)//following block is a copy-paste from the given libcurl example
		{
			wdi.size = 1024;
			/* Check for malloc failure in real code. */
			wdi.data = (char*)malloc(wdi.size);

			/* Set the URL for the operation. */
			curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());

			/* "write_data" function to call with returned data. */
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

			/* userp parameter passed to write_data. */
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wdi);

			/* Actually perform the query. */
			res = curl_easy_perform(curl);

			/* Check the return value and do whatever. */

			/* Clean up after ourselves. */
			curl_easy_cleanup(curl);
		}
		else
		{
			fprintf(stderr, "Error: could not get CURL handle.\n");
			exit(EXIT_FAILURE);
		}

		string* page_ptr = new string(wdi.data, wdi.len);

		/* cleanup wdi.data buffer. */
		free(wdi.data);

		//cout << "TEST" << endl;
		//_return = URL;

		return page_ptr;
	}

	int32_t hit_count;
	int32_t miss_count;
	int32_t network_volume;
	int32_t cached_volume;


public:
	proxyHandler(string& policy, unsigned int cache_size)
	{
		hit_count=0;
		miss_count=0;
		network_volume=0;
		cached_volume=0;

		if(policy == "FIFO")
		{
			cout << "Cache policy: FIFO (" << cache_size << " Bytes)" << endl<< endl;

			cache = new FIFOCache(cache_size);
		}
		else if(policy == "Random")
		{
			cout << "Cache policy: Random (" << cache_size << " Bytes)" << endl<< endl;

			cache = new RandomCache(cache_size);
		}
		else if(policy == "LRU")
		{
			cout << "Cache policy: LRU (" << cache_size << " Bytes)" << endl<< endl;

			cache = new LRUCache(cache_size);
		}
		else if(policy == "LFU")
		{
			cout << "Cache policy: LFU (" << cache_size << " Bytes)" << endl<< endl;

			cache = new LFUCache(cache_size);
		}
		else
		{
			cout << "Cache policy: No cache! "  << endl << endl;

			cache = new NoCache(cache_size);
		}
	}

	~proxyHandler()
	{
		delete cache;
	}

	void getPage(string& _return, const string& URL)
	{
		#if DEBUG_MESSAGES == 1
			cout << "Page requested: '" << URL << "'"<< endl;
		#endif

		string* page_ptr = cache->find(URL);

		if(page_ptr !=  NULL)//page found in local cache
		{
			hit_count++;
			cached_volume += page_ptr->size();

			#if DEBUG_MESSAGES == 1
				cout << "	Found requested page in cache: '" << URL << "\' ("  << page_ptr->size() << " Bytes)"<< endl;
			#endif

			_return	= *page_ptr;
		}
		else//page not found in local cache
		{
			page_ptr = loadPage(URL);//load it from the web

			if(page_ptr !=  NULL)
				_return	= *page_ptr;

			#if DEBUG_MESSAGES == 1
				cout << "	Cache current size: " << cache->getCurrentSize() << " Bytes"<< endl;
				cout << "	Inserting page: '" << URL << "\' ("  << _return.size() << " Bytes)"<< endl;
			#endif

			++miss_count;
			network_volume += page_ptr->size();

			cache->add(URL, page_ptr);//put it in the cache
		}

		#if DEBUG_MESSAGES == 1
			cout << endl;
		#endif
	}

	int32_t getCachedVolume()
	{
		return cached_volume;
	}

	int32_t getNetworkVolume()
	{
		return network_volume;
	}

	int32_t getHitCount()
	{
		return hit_count;
	}

	int32_t getMissCount()
	{
		return miss_count;
	}

	void resetCounters() 
	{
		cached_volume=0;
		network_volume=0;
		hit_count=0;
		miss_count=0;
	}

private:
	Cache* cache;

};

//main generated by thrift (modified to take the port as an argument)
int main(int argc, char **argv)
{
	if(argc != 4)
	{
		cout << "Mandatory input parameters: port cache_policy cache_size" << endl;
		return -1;
	}

	int port = atoi(argv[1]);

	string policy = argv[2];

	shared_ptr<proxyHandler> handler(new proxyHandler(policy, atoi(argv[3])));
	shared_ptr<TProcessor> processor(new proxyProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(
			new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TSimpleServer server(processor, serverTransport, transportFactory,
			protocolFactory);
	server.serve();
	return 0;
}

