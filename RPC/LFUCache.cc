#include "LFUCache.h"

#include <iostream>
#include <map>
#include <stdlib.h>


using namespace std;

LFUCache::LFUCache(unsigned int size)
{
	MAXSIZE = size;
	current_size = 0;
	time = 0;
}

string* LFUCache::find(const string& URL)
{
	std::map<string,LFU_element>::iterator it = myCache.find(URL);

	if (myCache.end() == it)
	{
		return NULL;
	}
	else
	{
		time++;

		string* page_ptr = it->second.page_ptr;
		unsigned long long access_count = it->second.access_count+1;

		leastFrequentlyUsed.erase(make_pair(it->second.access_count, it->second.last_time));//erase (old time, URL) pair
		leastFrequentlyUsed.insert(make_pair(make_pair(access_count, time), URL));//insert (new_time, URL) pair

		LFU_element val;
		val.access_count=access_count;
		val.last_time=time;
		val.page_ptr = page_ptr;

		myCache.erase(it);
		myCache.insert(make_pair(URL, val));

		return page_ptr;
	}
}

void LFUCache::add(const string&URL, string* page_ptr)
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
		string toRemove = leastFrequentlyUsed.begin()->second;
		leastFrequentlyUsed.erase(leastFrequentlyUsed.begin());

		map<string, LFU_element>::iterator it = myCache.find(toRemove);

		current_size -= it->second.page_ptr->size();

		#if DEBUG_MESSAGES == 1
			cout << "	Removing page: '" << toRemove << "\' ("  << it->second.page_ptr->size() << " Bytes)"<< endl;
			cout << "	Size after insertion:" << current_size << " Bytes" << endl;
		#endif

		myCache.erase(it);
	}

	time++;
	leastFrequentlyUsed.insert(make_pair(make_pair(0,time), URL));

	LFU_element val;
	val.access_count=0;
	val.last_time=time;
	val.page_ptr = page_ptr;
	myCache.insert(make_pair(URL, val));
}

unsigned int LFUCache::getCurrentSize()
{
	return current_size;
}
