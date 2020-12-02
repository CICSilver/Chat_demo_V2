#include "include/EvHttpServer.h"
#include "include/EvHttpResp.h"
#include <signal.h>
#include "include/threadPool.hpp"
#include <iostream>

using namespace std;
using namespace tp;

void http_gen_cb(EvHttpResp *resp)
{
    cout << "thread starting..." << endl;
}

int main()
{
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        return 1;
    }

    threadPool pool(20);
    EvHttpServer serv("0.0.0.0", 80);
    serv.RegHandler(http_gen_cb);

    serv.Start();
}