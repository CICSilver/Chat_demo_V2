#include "include/EvHttpServer.h"
#include "include/EvHttpResp.h"
#include "include/json.hpp"
#include <signal.h>
#include <iostream>
#include <functional>
#include <map>
#include <memory>
#include <sstream>

using namespace std;
using namespace nlohmann;

#define HOME "./web/index.html"

//处理查询请求
void ResponseQuery(EvHttpResp *resp);
//处理文件请求
void ResponseFile(EvHttpResp *resp, string filePath);
//处理用户发送的信息
void ResponseMsg(EvHttpResp *resp);

void SetContenType(string fileType, EvHttpResp *resp);

void GetQueryParams(string src, map<string, string> &params);

string JsonToString(json &root);

void http_gen_cb(EvHttpResp *resp)
{
    cout << "==================================================" << endl;
    cout << resp->GetRequestUri() << endl;

    if(resp->GetRequestUri() == "/")
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
    map<string, string> loginParams;

    GetQueryParams(str_body, loginParams);
    json JrespData;
    for (auto const &p : loginParams)
    {
        JrespData[p.first] = p.second;
    }
    string respData = JsonToString(JrespData);
    cout << respData << endl;
    resp->AddRespBuf(respData.c_str(), respData.length());
    resp->SetRespCode(200);
    resp->SendResponse();

    // string filePath = "." + resp->GetRequestUri() + ".html";
    // ResponseFile(resp, filePath);
}

void ResponseMsg(EvHttpResp *resp)
{
    string str_body = resp->GetPostMsg();
    cout << str_body << endl;

    map<string, string> msgParams;
    GetQueryParams(str_body, msgParams);

    json JmsgParams;
    for(auto const& p:msgParams)
    {
        JmsgParams[p.first] = p.second;
    }

    string respMsg = JsonToString(JmsgParams);
    resp->AddRespBuf(respMsg.c_str(), respMsg.length());
    resp->SetRespCode(200);
    resp->SendResponse();
}

void GetQueryParams(string src, map<string, string> &params)
{
    if (src == "")
    {
        return;
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
        params.emplace(str1, str2);
    }
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

int main(int argc, char *args[])
{
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        return 1;
    }

    //默认绑定0.0.0.0:80, timeout = 5000
    EvHttpServer serv("0.0.0.0", 80);

    serv.RegHandler("/web/login", ResponseLogin);
    serv.RegHandler("/web/recvMsg", ResponseMsg);
    serv.RegHandler(http_gen_cb);

    serv.Start();

    return 0;
}