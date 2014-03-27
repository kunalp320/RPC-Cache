#ifndef FIFO_H
#define FIFO_H

#include "cache.h"

class FIFOCache: public Cache
{
public:
	FIFOCache(unsigned int size);
	virtual ~FIFOCache(){}

	std::string* find(const std::string& URL);
	void add(const std::string& URL, std::string* page_ptr);

	unsigned int getCurrentSize();

private:
	void removeOnePage();
	void insertPage(const std::string& URL, std::string* page_ptr);

	unsigned int current_size;
	unsigned int max_size;
	std::queue<std::string> cache;//queue of the URLs that are in the map
	std::map<std::string, std::string*> URL_to_page;
};


#endif
