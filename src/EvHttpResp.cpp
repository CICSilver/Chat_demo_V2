#include "../include/EvHttpResp.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
EvHttpResp::EvHttpResp(struct evhttp_request *request) : evReq(request)
{
    // if(evReq == nullptr)
    // {
    //     return;
    // }
    //TODO : if failed, do sth
    evUri = evhttp_request_get_evhttp_uri(evReq);
    if (evUri)
    {
        const char *query = evhttp_uri_get_query(evUri);
        if (query)
        {
            evhttp_parse_query_str(query, &pathParams);
        }
    }
    requestHeaders = evhttp_request_get_input_headers(evReq);
    respHeaders = evhttp_request_get_output_headers(evReq);
    respBuf = evhttp_request_get_output_buffer(evReq);
}

std::string EvHttpResp::GetRequestUri()
{
    if (evReq)
    {
        const char *uri = evhttp_request_get_uri(evReq);
        if (uri)
        {
            return string(uri);
        }
    }
    return string("");
}
std::string EvHttpResp::GetUriHost()
{
    if (evReq)
    {
        const char *host = evhttp_request_get_host(evReq);
        if (host)
        {
            return string(host);
        }
    }
    return string("");
}
std::string EvHttpResp::GetUriPath()
{
    if (evUri)
    {
        const char *path = evhttp_uri_get_path(evUri);
        if (path)
        {
            return string(path);
        }
    }
    return string("");
}
std::string EvHttpResp::GetUriQuery()
{
    if (evUri)
    {
        const char *query = evhttp_uri_get_query(evUri);
        if (query)
        {
            return string(query);
        }
    }
    return string("");
}

int EvHttpResp::GetUriPort()
{
    return evhttp_uri_get_port(evUri);
}

std::string EvHttpResp::GetHeadParam(std::string const &strKey)
{
    if (requestHeaders)
    {
        const char *val = evhttp_find_header(requestHeaders, strKey.c_str());
        if (val)
        {
            return string(val);
        }
    }
    return string("");
}
std::string EvHttpResp::GetPathParam(std::string const &strKey)
{
    const char *val = evhttp_find_header(&pathParams, strKey.c_str());
    if (val)
    {
        return string(val);
    }
    return string("");
}
std::string EvHttpResp::GetPostParam(std::string const &strKey)
{
    const char *val = evhttp_find_header(&postParams, strKey.c_str());
    if (val)
    {
        return string(val);
    }
    return string("");
}
std::string EvHttpResp::GetPostMsg()
{
    if (!str_body.empty())
    {
        return str_body;
    }
    size_t len = 0;
    evbuffer *input_buf = evhttp_request_get_input_buffer(evReq);
    len = evbuffer_get_length(input_buf);
    if (len > 0)
    {
        str_body.resize(len);
        str_body.assign(reinterpret_cast<const char *>(evbuffer_pullup(input_buf, -1)), len);
    }
    return str_body;
}
bool EvHttpResp::AddRespHeadParam(std::string const &key, std::string const &val)
{
    return evhttp_add_header(respHeaders, key.c_str(), val.c_str()) == 0;
}
void EvHttpResp::AddRespHeaders(HttpHeaders &headers)
{
    for (auto const &itk : headers)
    {
        auto valList = itk.second;
        for (auto const &itv : valList)
        {
            AddRespHeadParam(itk.first, itv);
        }
    }
}
bool EvHttpResp::AddRespString(std::string const &str)
{
    return evbuffer_add_printf(respBuf, str.c_str()) != -1;
}
bool EvHttpResp::AddRespBuf(void const *data, std::size_t len)
{
    auto freeBufCB = [](const void *data, size_t datalen, void *extra) {
        ::operator delete(const_cast<void *>(data));
    };
    void *dataCopy = ::operator new(len);
    std::memcpy(dataCopy, data, len);
    if (evbuffer_add_reference(respBuf, dataCopy, len, freeBufCB, nullptr) != 0)
    {
        ::operator delete(dataCopy);
        return false;
    }
    return true;
}
bool EvHttpResp::AddRespFile(std::string const &fileName)
{
    int fd = open(fileName.c_str(), 0);
    if (fd == -1)
    {
        return false;
    }
    ev_off_t len = lseek(fd, 0, SEEK_END);

    if (evbuffer_add_file(respBuf, fd, 0, len) == -1)
    {
        return false;
    }
    close(fd);
    return true;
}
void EvHttpResp::SetRespCode(int code)
{
    resCode = code;
}
void EvHttpResp::SendResponse()
{
    evhttp_send_reply(evReq, resCode, nullptr, respBuf);
}
void EvHttpResp::SendResponse(int code, std::string const &strBody)
{
    if (!strBody.empty())
    {
        AddRespString(strBody);
    }
    evhttp_send_reply(evReq, code, nullptr, respBuf);
}
void EvHttpResp::SendResponse(int code, HttpHeaders &headers, std::string strBody)
{
    if (!strBody.empty())
    {
        AddRespString(strBody);
    }
    if (!headers.empty())
    {
        AddRespHeaders(headers);
    }
    evhttp_send_reply(evReq, code, nullptr, respBuf);
}
void EvHttpResp::RespError(int nCode, std::string const &strMsg)
{
    const char *res = nullptr;
    if (!strMsg.empty())
    {
        res = strMsg.c_str();
    }
    evhttp_send_error(evReq, nCode, res);
}