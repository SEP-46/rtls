#include "restapi.h"
#include "httplib.h"

void StartWebServer(RTLS& rtls)
{
    using namespace httplib;

    Server svr;

    svr.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!", "text/plain");
    });


    svr.listen("localhost", 80);    //setting port to 80
}