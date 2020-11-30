CFLAG = -I /home/ben/lib/libevent/include -L /home/ben/lib/libevent/lib 

MY_INCLUDE = /home/ben/Chat_demo_V2/include
MY_SRC = /home/ben/Chat_demo_V2/src

COM = /home/ben/Chat_demo_V2/com/

COM_DAO = /home/ben/Chat_demo_V2/com/dao/
COM_MODEL = /home/ben/Chat_demo_V2/com/model/
COM_UTILS = /home/ben/Chat_demo_V2/com/utils/

CLIBS = -levent -lsqlite3

SERVER_OBJ = http_server.o EvHttpResp.o EvHttpServer.o

http_server:$(SERVER_OBJ)
	g++ $(CFLAG) $^ -o $@ $(CLIBS)

http_server.o:http_server.cpp $(MY_INCLUDE)/EvHttpServer.h $(MY_INCLUDE)/EvHttpResp.h
	g++ $(CFLAG) -c http_server.cpp

EvHttpServer.o:$(MY_SRC)/EvHttpServer.cpp $(MY_INCLUDE)/EvHttpServer.h
	g++ $(CFLAG) -c $(MY_SRC)/EvHttpServer.cpp

EvHttpResp.o:$(MY_SRC)/EvHttpResp.cpp $(MY_INCLUDE)/EvHttpResp.h
	g++ $(CFLAG) -c $(MY_SRC)/EvHttpResp.cpp

# messageDAO.o:$(MY_DAO)messageDAO.cpp messageDAO.h messageModel.h $(COM_UTILS)dbUtils.h
# 	g++ $(CFLAG) -c $(MY_DAO)messageDAO.cpp

# userDAO.o:$(MY_DAO)userDAO.cpp userDAO.h userModel.h $(COM_UTILS)dbUtils.h
# 	g++ $(CFLAG) -c $(MY_DAO)userDAO.cpp

# messageModel.o:$(COM_MODEL)messageModel.cpp $(COM_MODEL)messageModel.h
# 	g++ $(CFLAG) -c $(COM_MODEL)messageModel.cpp

# userModel.o:$(COM_MODEL)userModel.cpp $(COM_MODEL)userModel.h
# 	g++ $(CFLAG) -c $(COM_MODEL)userModel.cpp


.PHONY:clean
clean:
	rm -rf *.o http_server



