#include "../include/EvHttpServer.h"
#include "../include/EvHttpResp.h"
#include <algorithm>
#include <sstream>
#include <string.h>
using namespace std;

std::string EvHttpServer::GetPostfix(std::string filePath)
{

    auto pointPos = filePath.rfind('.', filePath.size());
    auto fileType = filePath.substr(pointPos + 1, (filePath.size() - pointPos - 1));
    return fileType;
}

int EvHttpServer::stringToInt(std::string src)
{
    if (src == "")
    {
        return 0;
    }
    std::stringstream ss;
    int ret;
    ss << src;
    ss >> ret;
    return ret;
}

std::string EvHttpServer::GetValueFromHeader(evkeyvalq *evheader, std::string key)
{
    string val = "";
    if (key == "" || evheader == nullptr)
    {
        return val;
    }
    auto queueHead = evheader->tqh_first;
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    while (queueHead)
    {
        string curKey = queueHead->key;
        transform(curKey.begin(), curKey.end(), curKey.begin(), ::tolower);
        if (curKey == key)
        {
            val = queueHead->value;
            return val;
        }
        queueHead = queueHead->next.tqe_next;
    }
    return val;
}

bool EvHttpServer::RegHandler(std::string const &strUrl, handle_t *funcPtr)
{
    HandlerFunc func = funcPtr;
    if (!func)
    {
        return false;
    }

    auto transFunc = [](struct evhttp_request *req, void *arg) {
        if (req == nullptr)
        {
            cerr << "evh request handler is NULL" << endl;
            return;
        }
        EvHttpResp httpReq(req);
        handle_t *f = reinterpret_cast<handle_t *>(arg);
        f(&httpReq);
    };

    handle_t **pph = func.target<handle_t *>();
    if (pph != nullptr)
    {
        if (strUrl == "")
        {
            evhttp_set_gencb(evh, transFunc, reinterpret_cast<void *>(*pph));
            return true;
        }
        else
        {
            return (evhttp_set_cb(evh, strUrl.c_str(), transFunc, reinterpret_cast<void *>(*pph)) != -2);
        }
    }
    return false;
}

bool EvHttpServer::RegHandler(handle_t *funcPtr)
{
    return RegHandler("", funcPtr);
}

bool EvHttpServer::UnRegHandler(std::string const *strUrl)
{
    return (0 == evhttp_del_cb(evh, strUrl->c_str()));
}

bool EvHttpServer::Start()
{
    return (event_base_dispatch(base) == 0);
}
