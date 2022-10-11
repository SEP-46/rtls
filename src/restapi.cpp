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

    svr.Get( "/tag", [&]( const Request& req, Response& res ) {
        json tagJson;
        tagJson["name"] = rtls.GetTagName();

        res.set_content( tagJson.dump(), "application/json" );
    });

    svr.Get("/anchors", [&](const Request& req, Response& res) {
        auto anchors = rtls.GetAnchors();

        json anchorsJson = json::array();
        for (const AnchorConfig& anchor : anchors)
        {
            json anchorJson;
            anchorJson["id"] = anchor.id;
            anchorJson["name"] = anchor.name;
            json pos;
            pos["x"] = anchor.pos.x;
            pos["y"] = anchor.pos.y;
            pos["z"] = anchor.pos.z;
            anchorJson["pos"] = pos;
            anchorsJson.push_back(std::move(anchorJson));
        }

        res.set_content(anchorsJson.dump(), "application/json");
    });

    svr.Put(R"(/anchors/(\d+))", [&](const Request& req, Response& res) {
        NodeId_t id = (NodeId_t)std::stoi( req.matches[1].str() );

        json data = json::parse(req.body);
        Vec3 pos;
        pos.x = data["x"];
        pos.y = data["y"];
        pos.z = data["z"];

        if ( !rtls.SetAnchorPos( id, pos ) || !rtls.SetAnchorName( id, data["name"] ) )
        {
            json response;
            response["status"] = "error";
            res.status = 400;
            res.set_content( response.dump(), "application/json" );
        }

        json response;
        response["status"] = "success";
        res.set_content(response.dump(), "application/json");
    });

    svr.Get("/bounds", [&](const Request& req, Response& res) {
        const AABB& bounds = rtls.GetBounds();

        json mins;
        mins["x"] = bounds.mins.x;
        mins["y"] = bounds.mins.y;
        mins["z"] = bounds.mins.z;
        json maxs;
        maxs["x"] = bounds.maxs.x;
        maxs["y"] = bounds.maxs.y;
        maxs["z"] = bounds.maxs.z;
        json boundsJson;
        boundsJson["mins"] = std::move( mins );
        boundsJson["maxs"] = std::move( maxs );

        res.set_content( boundsJson.dump(), "application/json" );
    });

    svr.Put( "/bounds", [&]( const Request& req, Response& res ) {
        json data = json::parse( req.body );

        AABB bounds;
        bounds.mins.x = data["mins"]["x"];
        bounds.mins.y = data["mins"]["y"];
        bounds.mins.z = data["mins"]["z"];
        bounds.maxs.x = data["maxs"]["x"];
        bounds.maxs.y = data["maxs"]["y"];
        bounds.maxs.z = data["maxs"]["z"];

        rtls.SetBounds( bounds );

        json response;
        response["status"] = "success";
        res.set_content( response.dump(), "application/json" );
    } );

    svr.set_mount_point( "/", "../../web" );

    svr.listen("localhost", 80);    //setting port to 80
}