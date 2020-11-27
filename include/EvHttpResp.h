#ifndef _EVHTTPRESP_H
#define _EVHTTPRESP_H

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
#include <list>
#include <map>
#include <functional>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>

/**
* 1.获取请求的指定位置（首部、正文等）
* 2.判断请求的method类型
* 3.根据请求首部来判断响应的文件类型
* 4.设定响应首部，生成响应正文
* 5.一些辅助方法，诸如头部打印、string和int转换等
* 6.需要持有的一些成员(从请求中获取):evhttp_request,uri,路径，首部参数，端口参数，请求正文，响应头部，响应正文，响应状态码。
*/

typedef std::list<std::string> HttpHeadVal;
typedef std::map<std::string, HttpHeadVal> HttpHeaders;

class EvHttpResp
{
private:
    struct evhttp_request *evReq{nullptr};
    const struct evhttp_uri *evUri;
    struct evkeyvalq pathParams
    {
        0
    };
    struct evkeyvalq *requestHeaders{nullptr};
    struct evkeyvalq postParams
    {
        0
    };

    std::string str_body;
    struct evkeyvalq *respHeaders{nullptr};
    struct evbuffer *respBuf{nullptr};
    int resCode{HTTP_OK};

    std::string const WEBROOT = ".";
    std::string const DEFAULT_HOME = "index.html";

public:
    EvHttpResp(struct evhttp_request *request);
    ~EvHttpResp(){};

    std::string GetRequestUri();
    std::string GetUriHost();
    std::string GetUriPath();
    std::string GetUriQuery();

    int GetUriPort();

    std::string GetHeadParam(std::string const &strKey);
    std::string GetPathParam(std::string const &strKey);
    std::string GetPostParam(std::string const &strKey);
    std::string GetPostMsg();

    bool AddRespHeadParam(std::string const &key, std::string const &val);
    void AddRespHeaders(HttpHeaders &headers);
    bool AddRespString(std::string const &str);

    bool AddRespBuf(void const *data, std::size_t len);
    bool AddRespFile(std::string const &fileName);
    void SetRespCode(int code);
    void SendResponse();
    void SendResponse(int code, std::string const &strBody);
    void SendResponse(int code, HttpHeaders &headers, std::string strBody);
    void RespError(int nCode, std::string const &strMsg);

    void SetDefaultRespUri(std::string uri)
    {
    }

    std::string GetFilePath()
    {
        std::string uri = GetRequestUri();
        std::string filePath = WEBROOT;
        filePath += uri;
        if (strcmp(filePath.c_str(), "./web/") == 0)
        {
            filePath += DEFAULT_HOME;
        }
        return filePath;
    }

    std::string GetFileType(std::string filePath)
    {
        auto pointPos = filePath.rfind('.', filePath.size());
        auto fileType = filePath.substr(pointPos + 1, (filePath.size() - pointPos - 1));
        return fileType;
    }

    bool isFile()
    {
        std::string uri = GetRequestUri();
        if(uri == "/")
        {
            return true;
        }
        int ret = uri.rfind('.', uri.size());
        return (ret != -1);
    }

    bool isQuery()
    {
        return !GetUriQuery().empty();
    }

    void printInputHeader()
    {
        auto queueHead = requestHeaders->tqh_first;
        while (queueHead)
        {
            std::cout << queueHead->key << ":" << queueHead->value << std::endl;
            queueHead = queueHead->next.tqe_next;
        }
    }
};

#endif