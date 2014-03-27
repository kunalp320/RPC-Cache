#include "cache.h"

#include <iostream>

using namespace std;

bool NoCache::find(string& _return, const string& URL)
{
	return false;
}

void NoCache::add(string& _return, const string& URL)
{
	//nothing to do
}

FIFOCache::FIFOCache(unsigned int size): current_size(0), max_size(size)
{

}

bool FIFOCache::find(string& _return, const string& URL)
{
	map<string,string>::iterator it = URL_to_page.find(URL);

	if(it != URL_to_page.end())
	{
		_return = it->second;
		return true;
	}

	return false;
}

void FIFOCache::add(string& _return, const string& URL)
{
	if(_return.size() > max_size)
	{
		#if DEBUG_MESSAGES == 1
			cout << '\'' << URL << "' is too big for cache: " << _return.size() << endl;
		#endif

		return;//impossible to cache
	}

	while(current_size + _return.size() > max_size)
	{
		string URL = cache.front();

		map<string,string>::iterator it = URL_to_page.find(URL);

		if(it == URL_to_page.end())
		{
			cout << "Error in FIFOCache: queue and map don't agree" << endl;
		}

		//update current_size
		current_size-= it->second.size();

		//remove URL from queue
		cache.pop();

		#if DEBUG_MESSAGES == 1
			cout << "		REMOVE: '" << it->first << "\' ("  << it->second.size() << " Bytes)"<< endl;
		#endif

		//remove page from map
		URL_to_page.erase(it);
	}

	URL_to_page[URL] = _return;
	cache.push(URL);

	//update current_size
	current_size+= _return.size();
}
