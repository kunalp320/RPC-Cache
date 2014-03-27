#ifndef RANDOM_H
#define RANDOM_H

#include "cache.h"

using namespace std;

class RandomCache:public Cache
{

private:
	map<string, string*> myCache;
	unsigned int MAXSIZE;
	unsigned int current_size;
	vector<string> myVector;

public:
	RandomCache(unsigned int size);
	virtual ~RandomCache(){}

	std::string* find(const std::string& URL);
	void add(const std::string& URL, std::string* page_ptr);

	unsigned int getCurrentSize();

};

#endif

