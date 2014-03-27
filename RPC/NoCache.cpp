#include "NoCache.h"

#include <iostream>

using namespace std;

//NoCache has a size of zero, always return NULL (= not found) on URL and immediately frees page_ptr when inserted

string* NoCache::find(const string& URL)
{
	return NULL;
}

unsigned int NoCache::getCurrentSize()
{
	return 0;
}

void NoCache::add(const string& URL, string* page_ptr)
{
	delete page_ptr;
}

