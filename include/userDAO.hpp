#ifndef _USERDAO_HPP
#define _USERDAO_HPP

#include "sqlite3pp.h"
#include "sqlite3ppext.h"
#include "userModel.hpp"
#include "string"
#include <random>
#include <sstream>
#include <sys/time.h>
#include "json.hpp"
#include <vector>
#include <cstdio>

class userDAO
{
private:
    sqlite3pp::database db;
    std::string tableName = "user";
    std::vector<nlohmann::json> result;

public:
    userDAO()
    {
        const std::string databasePath = "/home/ben/Chat_demo_V2/db/chat.db";
        // sqlite3pp::database db_t(databasePath.c_str());
        // db = &db_t;
        db = sqlite3pp::database(databasePath.c_str());
    };
    ~userDAO();

    int Insert(User u);
    int Update(User u);
    int Delete(User u);
    //查询所有用户信息
    std::vector<nlohmann::json> Select();

private:
    std::string getUID()
    {
        std::stringstream ss;
        std::uniform_int_distribution<unsigned> uid_4(1000, 9999);
        std::random_device rd;
        std::string postfix;
        std::string prefix = getUid_prefix();
        ss << uid_4(rd);
        ss >> postfix;

        return (prefix + postfix);
    }

    std::string getUid_prefix()
    {
        timeval tv;
        std::stringstream ss;

        std::string u;
        int temp;
        do
        {
            gettimeofday(&tv, NULL);
        } while ((temp = tv.tv_usec / 100000) < 0);
        ss << tv.tv_usec;
        ss >> u;
        return u;
    }
};

userDAO::~userDAO()
{
}

/**
 * CREATE TABLE user(
 * id integer PRIMARY KEY,
 * uid text,
 * userName text,
 * userEmail text,
 * userPhone text,
 * passwd text,
 * province text,
 * city text
 * );
 */

int userDAO::Insert(User u)
{
    std::string uid = getUID();
    printf("111\n");
    std::string sql = "insert into " + tableName + " (uid,userName,userEmail,userPhone,passwd,province,city) values (?,?,?,?,?,?,?)";
    printf("222\n");
    sqlite3pp::command cmd(db, sql.c_str());
    printf("333\n");
    cmd.binder() << uid << u.GetUserName() << u.GetUserEmail() << u.GetUserPhone() << u.GetPasswd() << u.GetProvince() << u.GetCity();
    cmd.execute();
    std::cout << "insert done.." << std::endl;
}
int userDAO::Update(User u)
{
    //TODO
}
int userDAO::Delete(User u)
{
    // TODO
}

/**
 * uid text,
 * userName text,
 * userEmail text,
 * userPhone text,
 * passwd text,
 * province text,
 * city text
 * 
 */

std::vector<nlohmann::json> userDAO::Select()
{
    std::string sql = "select * from " + tableName;
    sqlite3pp::query qry(db, sql.c_str());
    std::vector<std::string> text = {"id", "uid", "userName", "userEmail", "userPhone", "passwd", "province", "city"};
    for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
    {
        //取出所有记录
        std::cout << "a" << std::endl;
        nlohmann::json user_json;
        for (int j = 0; j < qry.column_count(); ++j)
        {
            //按列输出
            std::cout << "b" << std::endl;
            user_json[text[j]] = (*i).get<char const *>(j);
        }
        result.emplace_back(user_json);
    }

    std::cout << "select done.." << std::endl;
    return result;
    // TODO
}

#endif