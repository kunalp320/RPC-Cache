#include "LRUCache.h"

#include <iostream>
#include <map>
#include <stdlib.h>

LRUCache::LRUCache(unsigned int size)
{
	MAXSIZE = size;
	current_size = 0;
	time = 0;
}

std::string* LRUCache::find(const std::string& URL)
{
	std::map<string,pair<unsigned long long int,string *> >::iterator it = myCache.find(URL);

	if (myCache.end() == it)
	{
		return NULL;
	}
	else
	{
		time++;

		string* page_ptr = it->second.second;


		leastRecentlyUsed.erase(it->second.first);//erase (old time, URL) pair
		leastRecentlyUsed.insert(pair<unsigned long long, string>(time, URL));//insert (new_time, URL) pair

		myCache.erase(it);
		myCache.insert(make_pair(URL, make_pair(time, page_ptr)));

		return page_ptr;
	}
}
void LRUCache::add(const std::string&URL, std::string* page_ptr)
{
	//if the page is bigger than the total size of the cache, it can't be inserted
	if(page_ptr->size() > MAXSIZE)
	{
		#if DEBUG_MESSAGES == 1
			cout << '\'' << URL << "' is too big for cache: " << page_ptr->size() << endl;
		#endif

		return;//impossible to cache
	}

	current_size += page_ptr->size(); //add the page of the inserted page to the total size

	#if DEBUG_MESSAGES == 1
		cout << "	Size after insertion: " << current_size << " Bytes" << endl;
	#endif

	while (current_size > MAXSIZE)
	{
		string toRemove = leastRecentlyUsed.begin()->second;
		leastRecentlyUsed.erase(leastRecentlyUsed.begin());

		map<string,pair<unsigned long long int,string *> >::iterator it = myCache.find(toRemove);

		current_size -= it->second.second->size();

		#if DEBUG_MESSAGES == 1
			cout << "	Removing page: '" << toRemove << "\' ("  << it->second.second->size() << " Bytes)"<< endl;
			cout << "	Size after insertion:" << current_size << " Bytes" << endl;
		#endif

		myCache.erase(it);
	}

	time++;
	leastRecentlyUsed.insert(make_pair(time, URL));
	myCache.insert(make_pair(URL, make_pair(time, page_ptr)));
}

unsigned int LRUCache::getCurrentSize()
{
	return current_size;
}
