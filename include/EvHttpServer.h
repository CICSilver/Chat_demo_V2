#ifndef _EVHTTPSERVER_H
#define _EVHTTPSERVER_H

#include "EvHttpResp.h"
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>
#include <event2/buffer.h>
#include <string>
#include <iostream>
#include <functional>

enum http_method_type
{
    METH_GET = 1 << 0,
    METH_POST = 1 << 1,
    METH_HEAD = 1 << 2,
    METH_PUT = 1 << 3,
    METH_DELETE = 1 << 4,
    METH_OPTIONS = 1 << 5,
    METH_TRACE = 1 << 6,
    METH_CONN = 1 << 7,
    METH_PATCH = 1 << 8
};
typedef void(handle_t)(EvHttpResp *);

class EvHttpServer
{
private:
    event_base *base;
    evhttp *evh;
    std::string servAddr;
    u_int16_t servPort;
    int timeOut;

    void static evLogCb(int severity, const char *msg)
    {
        switch (severity)
        {
        case EVENT_LOG_DEBUG:
            std::cerr << "[evLogCb] : EVENT_LOG_DEBUG" << std::endl;
            break;
        case EVENT_LOG_MSG:
            std::cerr << "[evLogCb] : EVENT_LOG_MSG" << std::endl;
            break;
        case EVENT_LOG_WARN:
            std::cerr << "[evLogCb] : EVENT_LOG_WARN" << std::endl;
            break;
        case EVENT_LOG_ERR:
            std::cerr << "[evLogCb] : EVENT_LOG_ERR" << std::endl;
            break;
        default:
            break;
        }
    }

public:
    typedef std::function<handle_t> HandlerFunc;
    EvHttpServer(std::string addr = "0.0.0.0", u_int16_t port = 80, int timeOut = 5000) : servAddr(addr), servPort(port), timeOut(timeOut)
    {
        base = event_base_new();
        if (!base)
        {
            std::cerr << "event_base_new failed" << std::endl;
        }
        evh = evhttp_new(base);

        int ret = evhttp_bind_socket(evh, servAddr.c_str(), servPort);

        EvHttpServer::SetTimeOut(timeOut);

        event_set_log_callback(evLogCb);
    };

    ~EvHttpServer()
    {
        if (base)
        {
            event_base_free(base);
        }
        if (evh)
        {
            evhttp_free(evh);
        }
    };

    void SetAlloweMethed(http_method_type method)
    {
        evhttp_set_allowed_methods(evh, method);
    }
    void SetTimeOut(int time)
    {
        evhttp_set_timeout(evh, time);
    }
    void SetMaxheaderSize(size_t num)
    {
        evhttp_set_max_headers_size(evh, num);
    }
    void SetMaxBodySize(size_t num)
    {
        evhttp_set_max_body_size(evh, num);
    }
    // void SetGenHandler(struct evhttp *http,void (*cb)(struct evhttp_request *, void *), void *arg);

    std::string GetValueFromHeader(evkeyvalq *evheader, std::string key);

    bool RegHandler(std::string const &strUrl, handle_t *funcPtr);
    bool RegHandler(handle_t *funcPtr);
    bool UnRegHandler(std::string const *strUrl);

    bool Start();

private:
    std::string GetPostfix(std::string filePath);
    int stringToInt(std::string src);
};

#endif