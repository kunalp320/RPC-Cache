#!/usr/local/bin/thrift --gen cpp

namespace cpp Proxy

service proxy 
{
string getPage(1:binary URL)

i32 getCachedVolume()
i32 getNetworkVolume()
i32 getHitCount()
i32 getMissCount()
void resetCounters()

}
