#ifndef _USERMODEL_HPP
#define _USERMODEL_HPP

#include <string>
#include <vector>
#include <iostream>

class User
{
private:
    std::string UserId = "0";
    std::string UserName;
    std::string UserEmail;
    std::string UserPhone;
    std::string Passwd;
    std::string Province;
    std::string City;

    int isManager = -1;

public:
    User(std::string _userName, std::string _userEmail, std::string _userPhone, std::string _passwd, std::string _province, std::string _city) : UserName(_userName), UserEmail(_userEmail), UserPhone(_userPhone), Passwd(_passwd), Province(_province), City(_city) {}
    User(User *u)
    {
        this->UserName = u->UserName;
        this->UserEmail = u->UserEmail;
        this->UserPhone = u->UserPhone;
        this->Passwd = u->Passwd;
        this->Province = u->Province;
        this->City = u->City;
    }
    User(std::vector<std::string> userParams)
    {
        if (userParams.size() != 6)
        {
            std::cerr << "user initilized failed.." << std::endl;
            return;
        }
        User(userParams[0], userParams[1], userParams[2], userParams[3], userParams[4], userParams[5]);
    }
    User(nlohmann::json Jparams)
    {   
        const std::string paramNames[6] = {"userName", "userEmail", "userPhone", "passwd", "province", "city"};
        this->UserName = Jparams[paramNames[0]];
        this->UserEmail = Jparams[paramNames[1]];
        this->UserPhone = Jparams[paramNames[2]];
        this->Passwd = Jparams[paramNames[3]];
        this->Province = Jparams[paramNames[4]];
        this->City = Jparams[paramNames[5]];
    };
    ~User();

    void SetUserId(int userId)
    {
        this->UserId = userId;
    }
    void SetUserName(std::string userName)
    {
        this->UserName = userName;
    }
    void SetUserEmail(std::string userEmail)
    {
        this->UserEmail = userEmail;
    }
    void SetUserPhone(std::string userPhone)
    {
        this->UserPhone = userPhone;
    }
    void SetPasswd(std::string passwd)
    {
        this->Passwd = passwd;
    }
    void SetProvince(std::string province)
    {
        this->Province = province;
    }
    void SetCity(std::string city)
    {
        this->City = city;
    }

    std::string GetUserId()
    {
        return this->UserId;
    }
    std::string GetUserName()
    {
        return this->UserName;
    }
    std::string GetUserEmail()
    {
        return this->UserEmail;
    }
    std::string GetUserPhone()
    {
        return this->UserPhone;
    }
    std::string GetPasswd()
    {
        return this->Passwd;
    }
    std::string GetProvince()
    {
        return this->Province;
    }
    std::string GetCity()
    {
        return this->City;
    }
    int getIsManager()
    {
        return this->isManager;
    }
};

User::~User()
{
    
}

#endif