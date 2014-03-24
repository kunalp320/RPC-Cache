#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <queue>
#include <map>

class Cache
{
public:
	Cache(unsigned int size){}//maximal sum of the sizes of all pages in the cache
	virtual ~Cache(){}

	virtual std::string* find(const std::string& URL)=0;//return the page address if the URL is found, else NULL
	virtual void add(const std::string& URL, std::string* page_ptr)=0;//add the (URL, page_ptr) pair to the cache

	virtual unsigned int getCurrentSize()=0;//returns the current size of the cache

protected:
	Cache(){}//implicitly called by children classes in their own constructors
};

#endif
