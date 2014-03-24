#include "RandomCache.h"

#include <iostream>
#include <stdlib.h>
#include <map>

#include <ctime>

RandomCache::RandomCache(unsigned int size)
{
	MAXSIZE = size;
	current_size = 0;

	srand(time(NULL));//init random generator
}

std::string* RandomCache::find(const std::string& URL)
{
	if (myCache.end() == myCache.find(URL))
	{
		return NULL;
	}
	else
	{
		return myCache.find(URL)->second;
	}
}

void RandomCache::add(const std::string& URL, std::string* page_ptr)
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
		int random = rand() % myVector.size();

		string toRemove = myVector.at(random);

		//if we erase in the middle, all the elements after the removed one are moved (O(n))

		myVector.at(random) = myVector.back();//copy the last element of the vector in myVector[random]
		myVector.pop_back();//remove the last element (O(1))

		std::string* removed_page = myCache.find(toRemove)->second;

		current_size -= removed_page->size();//remove the size of the evicted page from the total size

		#if DEBUG_MESSAGES == 1
			cout << "	Removing page: '" << toRemove << "\' ("  << removed_page->size() << " Bytes)"<< endl;
			cout << "	Size after insertion:" << current_size << " Bytes" << endl;
		#endif

		myCache.erase(toRemove);

		delete removed_page;
	}

	myCache.insert(make_pair(URL, page_ptr));
	myVector.push_back(URL);
}

unsigned int RandomCache::getCurrentSize()
{
	return current_size;
}
