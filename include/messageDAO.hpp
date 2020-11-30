#ifndef _MESSAGEDAO_HPP
#define _MESSAGEDAO_HPP

#include "messageModel.hpp"
#include "sqlite3pp.h"
#include "sqlite3ppext.h"

class messageDAO
{
private:
    sqlite3pp::database db;
    std::string tableName = "message_all_0";

public:
    messageDAO() { }
    messageDAO(std::string table = "message_all_0") : tableName(table)
    {
        const std::string databasePath = "../db/chat.db";
        db = sqlite3pp::database(databasePath.c_str());
    };
    ~messageDAO() {};
    int Insert(Message m);
    int Update(Message m);
    int Delete(Message m);
};

int messageDAO::Insert(Message m)
{
    std::string sql = "insert into " + tableName + " (from_,msg_,to_) values(?,?,?)";
    sqlite3pp::command cmd(db, sql.c_str());
    cmd.binder() << m.GetFrom() << m.GetMsg() << m.GetTo();
    cmd.execute();
}
int messageDAO::Update(Message m)
{
    // TODO
}
int messageDAO::Delete(Message m)
{
    // TODO
}


#endif