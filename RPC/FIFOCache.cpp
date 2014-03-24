#include "FIFOCache.h"

#include <iostream>

using namespace std;

//FIFOCache uses a queue to decide which pages to evict

FIFOCache::FIFOCache(unsigned int size): current_size(0), max_size(size)
{

}

//returns the pointer to the page corresponding to the URL or NULL if not found in the cache
string* FIFOCache::find(const string& URL)
{
	map<string,string*>::iterator it = URL_to_page.find(URL);

	if(it != URL_to_page.end())
	{
		return it->second;
	}

	return NULL;
}

//get the current sum of all sizes of the pages in the cache
unsigned int FIFOCache::getCurrentSize()
{
	return current_size;
}

void FIFOCache::removeOnePage()
{
	string URL = cache.front();//first inserted

	map<string,string*>::iterator it = URL_to_page.find(URL);

	if(it == URL_to_page.end())
	{
		cout << "Error in FIFOCache: queue and map don't agree" << endl;
	}

	//update current_size: remove the size of the page to be evicted
	current_size-= it->second->size();

	//remove the URL of the evicted page from queue
	cache.pop();

	#if DEBUG_MESSAGES == 1
		cout << "	Removing page: '" << it->first << "\' ("  << it->second->size() << " Bytes)"<< endl;
		cout << "	Size after insertion: " << current_size << " Bytes" << endl;
	#endif

	string* removed_page = it->second;

	//remove the evicted page from map
	URL_to_page.erase(it);

	delete removed_page;//delete the evicted page
}

void FIFOCache::insertPage(const string& URL, string* page_ptr)
{
	URL_to_page[URL] = page_ptr;//insert the <URL, page_ptr> pair in the map
	cache.push(URL);//push the URL in the FIFO
}

void FIFOCache::add(const string& URL, string* page_ptr)
{
	//if the page is bigger than the total size of the cache, it can't be inserted
	if(page_ptr->size() > max_size)
	{
		#if DEBUG_MESSAGES == 1
			cout << '\'' << URL << "' is too big for cache: " << page_ptr->size() << endl;
		#endif

		return;//impossible to cache
	}

	#if DEBUG_MESSAGES == 1
		cout << "	Size after insertion: " << current_size + page_ptr->size() << " Bytes" << endl;
	#endif


	//update current_size: add the size of the new page
	current_size+= page_ptr->size();

	//while the total size of the cache with the new page is over the limit, remove a page based on FIFO order
	while(current_size > max_size)
	{
		removeOnePage();
	}

	insertPage(URL, page_ptr);
}

