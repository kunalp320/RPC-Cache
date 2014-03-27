#ifndef LRUCACHE_H
#define LRUCACHE_H

#include "cache.h"

using namespace std;

class LRUCache:public Cache
{
	private:
		map<string, pair<unsigned long long, string*> > myCache;
		map<unsigned long long, string> leastRecentlyUsed;
		unsigned int current_size;
		unsigned int MAXSIZE;
		unsigned long long time;
	
	public:
		LRUCache(unsigned int size);
		virtual ~LRUCache(){}

		std::string* find(const std::string& URL);
		void add(const std::string& URL, std::string* page_ptr);

		unsigned int getCurrentSize();
};

#endif
