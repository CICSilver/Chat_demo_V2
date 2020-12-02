#ifndef _MESSAGEMODEL_HPP
#define _MESSAGEMODEL_HPP

#include <string>
#include "json.hpp"

class Message
{
private:
    std::string From;
    std::string Msg;
    std::string To;

public:
    Message(std::string from, std::string msg, std::string to) : From(from), Msg(msg), To(to) {}
    Message(Message *m)
    {
        Message(m->From, m->Msg, m->To);
    }
    Message(nlohmann::json Jparams){
        const std::string paramNames[3] = {"from","msg","to"};
        this->From = Jparams[paramNames[0]];
        this->Msg = Jparams[paramNames[1]];
        this->To = Jparams[paramNames[2]];

    };
    ~Message(){};

    void SetFrom(std::string from)
    {
        this->From = from;
    }
    void SetMsg(std::string msg)
    {
        this->Msg = msg;
    }
    void SetTo(std::string to)
    {
        this->To = to;
    }

    std::string GetFrom()
    {
        return this->From;
    }
    std::string GetMsg()
    {
        return this->Msg;
    }
    std::string GetTo()
    {
        return this->To;
    }
};

#endif