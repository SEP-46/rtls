#include "restapi.h"
#include "httplib.h"
#include <sstream>

void StartWebServer(RTLS& rtls)
{
    using namespace httplib;

    Server svr;

    svr.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    svr.Get("/anchors", [&](const Request& req, Response& res) {
        auto positions = rtls.GetAnchorPos();

        std::ostringstream ss;
        for (const Vec3& pos : positions)
        {
            ss << pos.x << ',' << pos.y << ',' << pos.y << '\n';
        }

        res.set_content(ss.str(), "text/plain");
    });

    svr.listen("localhost", 80);    //setting port to 80
}