CFLAG = -I /home/ben/lib/libevent/include -I /usr/local/include/json -L /home/ben/lib/libevent/lib 

MY_INCLUDE = /home/ben/Chat_demo_V2/include
MY_SRC = /home/ben/Chat_demo_V2/src

CLIBS = -levent



http_server:http_server.o EvHttpResp.o EvHttpServer.o
	g++ $(CFLAG) $^ -o $@ $(CLIBS)

http_server.o:http_server.cpp $(MY_INCLUDE)/EvHttpServer.h $(MY_INCLUDE)/EvHttpResp.h
	g++ $(CFLAG) -c http_server.cpp

EvHttpServer.o:$(MY_SRC)/EvHttpServer.cpp $(MY_INCLUDE)/EvHttpServer.h
	g++ $(CFLAG) -c $(MY_SRC)/EvHttpServer.cpp

EvHttpResp.o:$(MY_SRC)/EvHttpResp.cpp $(MY_INCLUDE)/EvHttpResp.h
	g++ $(CFLAG) -c $(MY_SRC)/EvHttpResp.cpp

.PHONY:clean
clean:
	rm -rf *.o



