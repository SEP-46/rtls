#include "restapi.h"

#include <httplib.h>
#include <json.hpp>

#include <sstream>

void StartWebServer(RTLS& rtls)
{
    using namespace httplib;
    using json = nlohmann::json;

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


    //setting anchor 0
    svr.Put("/anchors/0", [&](const Request& req, Response& res) {      
        json data = json::parse(req.body);      //gets xyz data
        Vec3 position;      //create a vector
        position.x = data["x"];
        position.y = data["y"];
        position.z = data["z"];

        rtls.SetAnchorPos(0, position);

        res.set_content("Success!", "text/plain");

    });
    
    //setting anchor 1
    svr.Put("/anchors/1", [&](const Request& req, Response& res) {
        json data = json::parse(req.body);      //gets xyz data
        Vec3 position;      //create a vector
        position.x = data["x"];
        position.y = data["y"];
        position.z = data["z"];

        rtls.SetAnchorPos(1, position);

        res.set_content("Success!", "text/plain");

    });

    //setting anchor 2
    svr.Put("/anchors/2", [&](const Request& req, Response& res) {
        json data = json::parse(req.body);      //gets xyz data
        Vec3 position;      //create a vector
        position.x = data["x"];
        position.y = data["y"];
        position.z = data["z"];

        rtls.SetAnchorPos(2, position);

        res.set_content("Success!", "text/plain");

    });

    //setting anchor 3
    svr.Put("/anchors/3", [&](const Request& req, Response& res) {
        json data = json::parse(req.body);      //gets xyz data
        Vec3 position;      //create a vector
        position.x = data["x"];
        position.y = data["y"];
        position.z = data["z"];

        rtls.SetAnchorPos(3, position);

        res.set_content("Success!", "text/plain");

    });

    svr.listen("localhost", 80);    //setting port to 80
}