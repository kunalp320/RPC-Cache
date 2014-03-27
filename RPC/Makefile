all: proxy_client proxy_server

proxy_client: client.o proxy.o constants.o types.o
	g++ -L/usr/local/lib client.o proxy.o constants.o types.o -o proxy_client -lthrift

client.o: proxy_client.cpp
	g++ -Wall -I/usr/local/include/thrift -c proxy_client.cpp -o client.o


proxy_server: proxy.o server.o constants.o types.o cache.o
	g++ -L/usr/local/lib proxy.o server.o constants.o types.o cache.o -o proxy_server -lthrift -lcurl

cache.o: cache.cpp
	g++ -Wall -I/usr/local/include/thrift -c cache.cpp -o cache.o

proxy.o: proxy.cpp
	g++ -Wall -I/usr/local/include/thrift -c proxy.cpp -o proxy.o

server.o: proxy_server.cpp
	g++ -Wall -I/usr/local/include/thrift -c proxy_server.cpp -o server.o

constants.o: proxy_server_constants.cpp
	g++ -Wall -I/usr/local/include/thrift -c proxy_server_constants.cpp -o constants.o

types.o: proxy_server_types.cpp 
	g++ -Wall -I/usr/local/include/thrift -c proxy_server_types.cpp -o types.o

clean:
	rm -f *.o *.html
