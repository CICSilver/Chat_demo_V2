#ifndef _MESSAGEMODEL_HPP
#define _MESSAGEMODEL_HPP

#include <string>

class Message
{
private:
    std::string From;
    std::string Msg;
    std::string To;

public:
    Message(std::string from, std::string msg, std::string to):From(from), Msg(msg), To(to) { }
    Message(Message *m){
        Message(m->From, m->Msg, m->To);
    };
    ~Message() { };

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