#ifndef LFUCACHE_H
#define LFUCACHE_H

#include "cache.h"

using namespace std;

struct LFU_element
{
	unsigned long long access_count;
	unsigned long long last_time;
	string* page_ptr;
};

class LFUCache:public Cache
{
	private:

		//map< URL, access_count, last_time, page_content>
		map<string, LFU_element> myCache;

		//map< (access_count, last_time), URL>
		map<pair<unsigned long long, unsigned long long>, string> leastFrequentlyUsed;
		unsigned int current_size;
		unsigned int MAXSIZE;
		unsigned long long time;
	
	public:
		LFUCache(unsigned int size);
		virtual ~LFUCache(){}

		std::string* find(const std::string& URL);
		void add(const std::string& URL, std::string* page_ptr);

		unsigned int getCurrentSize();
};

#endif
