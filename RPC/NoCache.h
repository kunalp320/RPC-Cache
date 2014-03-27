#ifndef NO_H
#define NO_H

#include "cache.h"

class NoCache: public Cache
{
public:
	NoCache(unsigned int size){}
	virtual ~NoCache(){}

	std::string* find(const std::string& URL);
	void add(const std::string& URL, std::string* page_ptr);

	unsigned int getCurrentSize();
};

#endif

