#include "include/EvHttpServer.h"
#include "include/EvHttpResp.h"
#include "include/json.hpp"
#include "include/userModel.hpp"
#include "include/userDAO.hpp"
#include "include/messageModel.hpp"
#include "include/messageDAO.hpp"
#include <vector>
#include <signal.h>
#include <iostream>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <thread>
#include "include/threadPool.hpp"
#include <condition_variable>

using namespace std;
using namespace nlohmann;

#define HOME "./web/index.html"

vector<thread> pool;
vector<string> recv_mems;
vector<string> waiting_mems;
condition_variable cv;
mutex m_lock;

//处理查询请求
void ResponseQuery(EvHttpResp *resp);
//处理文件请求
void ResponseFile(EvHttpResp *resp, string filePath);
//处理用户发送的信息
void ResponseMsg(EvHttpResp *resp);
//处理注册请求
void ResponseRegiste(EvHttpResp *resp);
//处理登录请求
void ResponseLogin(EvHttpResp *resp);
//处理获取历史记录请求
void ResponseGetHistory(EvHttpResp *resp);
//处理拉取指向自身的消息请求
void ResponseGetMsg(EvHttpResp *resp);

int stringToInt(string str);

void SetContenType(string fileType, EvHttpResp *resp);

nlohmann::json GetQueryParams(string src);

string JsonToString(json &root);

void http_gen_cb(EvHttpResp *resp)
{
    cout << "==================================================" << endl;
    cout << resp->GetRequestUri() << endl;

    if (resp->GetRequestUri() == "/")
    {
        ResponseFile(resp, HOME);
    }

    if (resp->isFile())
    {
        ResponseFile(resp, resp->GetFilePath());
    }

    if (resp->isQuery())
    {
        ResponseQuery(resp);
    }
}

void ResponseQuery(EvHttpResp *resp)
{
    string query = resp->GetUriQuery();
    cout << "GetUriQuery: " << query << endl;
}

void ResponseFile(EvHttpResp *resp, string filePath)
{
    string fileType = resp->GetFileType(filePath);
    // cout << filePath << endl;

    // resp->printInputHeader();

    SetContenType(fileType, resp);

    resp->AddRespFile(filePath);

    resp->SendResponse();
}

void ResponseLogin(EvHttpResp *resp)
{
    string str_body = resp->GetPostMsg();
    cout << str_body << endl;
    //userEmail, passwd
    json JrespData = GetQueryParams(str_body);

    userDAO ud;
    auto res = ud.Select(JrespData["userEmail"]);
    cout << JrespData["userEmail"] << endl;
    // if(res.size() != 1)
    // {
    //     cerr << "duplicate account" << endl;
    // }
    json jRes = res[0];
    string passwd = jRes["passwd"];
    if (jRes["passwd"] == JrespData["passwd"])
    {
        string respData = JsonToString(jRes);
        cout << passwd << endl;
        resp->AddRespBuf(respData.c_str(), respData.length());
        resp->SetRespCode(200);
    }
    else
    {
        resp->SetRespCode(100);
    }
    resp->SendResponse();

    // string filePath = "." + resp->GetRequestUri() + ".html";
    // ResponseFile(resp, filePath);
}

void ResponseRegiste(EvHttpResp *resp)
{
    const string paramNames[6] = {"userName", "userEmail", "userPhone", "passwd", "province", "city"};
    const string resParamNames[8] = {"id", "uid", "userName", "userEmail", "userPhone", "passwd", "province", "city"};
    string str_body = resp->GetPostMsg();

    json JregParams = GetQueryParams(str_body);

    User u(JregParams);
    userDAO ud;

    ud.Insert(u);

    json JrespData;
    JrespData["Code"] = "REG_OK";
    string respData = JsonToString(JrespData);
    resp->AddRespBuf(respData.c_str(), respData.length());
    resp->SetRespCode(200);
    resp->SendResponse();
}

void ResponseMsg(EvHttpResp *resp)
{
    //from,msg,to
    string str_body = resp->GetPostMsg();
    cout << str_body << endl;

    json JmsgParams = GetQueryParams(str_body);
    //接受消息的用户加入到等待唤醒的队列
    recv_mems.emplace_back(JmsgParams["to"]);

    Message m(JmsgParams);
    messageDAO md;

    md.Insert(m);

    string respMsg = JsonToString(JmsgParams);
    resp->AddRespBuf(respMsg.c_str(), respMsg.length());
    resp->SetRespCode(200);
    resp->SendResponse();
    cv.notify_all();
}

void ResponseGetHistory(EvHttpResp *resp)
{

    resp->SetRespCode(200);
    resp->SendResponse();
}

void GetMsg_t(EvHttpResp *resp)
{
    while (1)
    {
        string str_body = resp->GetPostMsg();
        cout << str_body << endl;
        json JmsgParams = GetQueryParams(str_body);
        unique_lock<mutex> u_lock(m_lock);
        cv.wait(u_lock, [&] {
            bool res = false;
            auto temp = recv_mems.end();
            for (auto it = recv_mems.begin(); it != recv_mems.end(); ++it)
            {
                if (*it == JmsgParams["to"])
                {
                    res = true;
                    temp = it;
                }
            }
            if (temp != recv_mems.end())
            {
                recv_mems.erase(temp);
            }
            return res;
        });

        int curID = stringToInt(JmsgParams["id"]);
        messageDAO md;
        vector<json> res;
        res = md.Select("", JmsgParams["to"]);

        if (res.size() == curID)
        {
            //当前显示的是最新消息
            json Jlatest = res[res.size() - 1];
            string respData = JsonToString(Jlatest["id"]);
            resp->AddRespBuf(respData.c_str(), respData.length());
        }
        else if (res.size() > curID)
        {
            //当前显示的记录id小于数据库中的记录id,显示curID之后的所有记录
            json Jlatest;
            int msgSize = res.size() - curID;
            // cout << res.size() << endl;
            // cout << curID << endl;
            Jlatest["msgSize"] = to_string(msgSize);
            int count = 0;
            for (int i = curID; i < res.size(); ++i)
            {
                Jlatest["msg"][count++] = res[i];
            }
            string respData = JsonToString(Jlatest);
            resp->AddRespBuf(respData.c_str(), respData.length());
        }
        resp->SetRespCode(200);
        resp->SendResponse();
    }
}

//仅处理第一聊天室的消息拉取
void ResponseGetMsg(EvHttpResp *resp)
{
    /*      即时响应部分        */
    // string str_body = resp->GetPostMsg();
    // cout << str_body << endl;
    // json JmsgParams = GetQueryParams(str_body);
    // //发起请求的用户已经在等待，则直接返回
    // for(auto const& m:waiting_mems)
    // {
    //     if(m == JmsgParams["to"])
    //     {
    //         return;
    //     }
    // }
    pool.emplace_back(thread(GetMsg_t, resp));
    auto p_it = pool.end() - 1;
    (*p_it).detach();

    /*      以下为轮询部分代码      */

    // int curID = stringToInt(JmsgParams["id"]);
    // messageDAO md;
    // vector<json> res;
    // // cout << JmsgParams["to"] << endl;
    // res = md.Select("", JmsgParams["to"]);

    // if (res.size() == curID)
    // {
    //     //当前显示的是最新消息
    //     json Jlatest = res[res.size() - 1];
    //     string respData = JsonToString(Jlatest["id"]);
    //     resp->AddRespBuf(respData.c_str(), respData.length());
    // }
    // else if (res.size() > curID)
    // {
    //     //当前显示的记录id小于数据库中的记录id,显示curID之后的所有记录
    //     json Jlatest;
    //     int msgSize = res.size() - curID;
    //     cout << res.size() << endl;
    //     cout << curID << endl;
    //     Jlatest["msgSize"] = to_string(msgSize);
    //     int count = 0;
    //     for (int i = curID; i < res.size(); ++i)
    //     {
    //         Jlatest["msg"][count++] = res[i];
    //     }
    //     string respData = JsonToString(Jlatest);
    //     resp->AddRespBuf(respData.c_str(), respData.length());
    // }
    // resp->SetRespCode(200);
    // resp->SendResponse();
}

nlohmann::json GetQueryParams(string src)
{
    json JParams;
    map<string, string> params;
    if (src == "")
    {
        cerr << "GetQueryParams failed: no str_body.." << endl;
        return JParams;
    }
    string temp = "?";
    temp += src;

    int aPos = 0;
    int ePos = 0;
    int curPos;

    while (aPos <= temp.length())
    {
        ePos = temp.find('=', aPos);
        string str1 = temp.substr(aPos + 1, (ePos - aPos - 1));
        aPos = temp.find('&', ePos);
        string str2 = temp.substr(ePos + 1, (aPos - ePos - 1));
        JParams[str1] = str2;
    }

    // for (auto const &p : params)
    // {
    //     JParams[p.first] = p.second;
    // }

    return JParams;
}

void SetContenType(string fileType, EvHttpResp *resp)
{
    if (fileType == "*")
    {
        resp->AddRespHeadParam("Content-Type", "application/octet-stream;charset=UTF8");
    }
    else if (fileType == "html" || fileType == "htm" || fileType == "htx" || fileType == "jsp" || fileType == "plg" || fileType == "stm" || fileType == "xhtml")
    {
        resp->AddRespHeadParam("Content-Type", "text/html;charset=UTF8");
    }
    else if (fileType == "js")
    {
        resp->AddRespHeadParam("Content-Type", "text/javascript;charset=UTF8");
    }
    else if (fileType == "css")
    {
        resp->AddRespHeadParam("Content-Type", "text/css;charset=UTF8");
    }
    else if (fileType == "jpg" || fileType == "gif" || fileType == "png")
    {
        string temp = "image/";
        temp += fileType;
        resp->AddRespHeadParam("Content-Type", temp.c_str());
    }
}

string JsonToString(json &root)
{
    stringstream ss;
    string res;
    ss << root;
    ss >> res;
    return res;
}

int stringToInt(string str)
{
    stringstream ss;
    int res;
    ss << str;
    ss >> res;
    return res;
}

int main(int argc, char *args[])
{
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        return 1;
    }

    //默认绑定0.0.0.0:80, timeout = 5000
    EvHttpServer serv("0.0.0.0", 80);
    ///web/getMsg

    serv.RegHandler("/web/getMsg", ResponseGetMsg);
    serv.RegHandler("/web/getHistory", ResponseGetHistory);
    serv.RegHandler("/web/reg", ResponseRegiste);
    serv.RegHandler("/web/login", ResponseLogin);
    serv.RegHandler("/web/recvMsg", ResponseMsg);
    serv.RegHandler(http_gen_cb);

    serv.Start();

    return 0;
}