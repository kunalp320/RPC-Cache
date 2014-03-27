#include <string>
#include <queue>
#include <map>

#define DEBUG_MESSAGES 1

class Cache
{
public:
	Cache(unsigned int size){}
	virtual ~Cache(){}

	virtual bool find(std::string& _return, const std::string& URL)=0;
	virtual void add(std::string& _return, const std::string& URL)=0;

protected:
	Cache(){}
};

class NoCache: public Cache
{
public:
	NoCache(unsigned int size){}
	virtual ~NoCache(){}

	bool find(std::string& _return, const std::string& URL);
	void add(std::string& _return, const std::string& URL);
};

class FIFOCache: public Cache
{
public:
	FIFOCache(unsigned int size);
	virtual ~FIFOCache(){}

	bool find(std::string& _return, const std::string& URL);
	void add(std::string& _return, const std::string& URL);

private:
	unsigned int current_size;
	unsigned int max_size;
	std::queue<std::string> cache;//queue of the URLs that are in the map
	std::map<std::string, std::string> URL_to_page;
};
