#ifndef _MESSAGEDAO_HPP
#define _MESSAGEDAO_HPP

#include "messageModel.hpp"
#include "sqlite3pp.h"
#include "sqlite3ppext.h"
#include <vector>

class messageDAO
{
private:
    sqlite3pp::database db;
    std::string tableName = "message_all_0";

public:
    messageDAO()
    {
        const std::string databasePath = "/home/ben/Chat_demo_V2/db/chat.db";
        db = sqlite3pp::database(databasePath.c_str());
    };
    ~messageDAO(){};
    int Insert(Message m);
    int Update(Message m);
    int Delete(Message m);
    // std::vector<nlohmann::json> Select();
    std::vector<nlohmann::json> Select(std::string from, std::string to);
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

//from_,msg_,to_
std::vector<nlohmann::json> messageDAO::Select(std::string from, std::string to)
{
    std::vector<nlohmann::json> result;
    std::string sql = "select * from " + tableName;
    bool flag = false;
    std::vector<std::string> text = {"id","from", "msg", "to"};
    if (from != "")
    {
        sql += " where from_='" + from + "'";
        flag = true;
    }
    if (to != "")
    {
        if (flag)
        {
            sql += " and to_='" + to + "'";
        }
        else
        {
            sql += " where to_='" + to + "'";
        }
    }
    // std::cout << sql << std::endl;
    sqlite3pp::query qry(db, sql.c_str());

    for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
    {
        //取出所有记录
        nlohmann::json Jmsg;
        for (int j = 0; j < qry.column_count(); ++j)
        {
            //按列输出
            Jmsg[text[j]] = (*i).get<char const *>(j);
        }
        result.emplace_back(Jmsg);
    }

    return result;
}

#endif