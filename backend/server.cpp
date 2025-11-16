#include "crow_all.h"
#include "json.hpp"
#include <curl/curl.h>
#include <thread>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
using json = nlohmann::json;

// ---------- helper ----------
size_t writeCB(void* c,size_t s,size_t n,std::string* o){
    o->append((char*)c,s*n); return s*n;
}
std::string httpGet(const std::string& url){
    CURL* curl=curl_easy_init(); std::string data;
    if(curl){
        curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,writeCB);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&data);
        curl_easy_setopt(curl,CURLOPT_USERAGENT,"MapNavigator/1.0");
        curl_easy_perform(curl); curl_easy_cleanup(curl);
    }
    return data;
}

// proper URL encoder (for spaces, accents, symbols)
std::string urlEncode(const std::string& value){
    std::ostringstream escaped; escaped.fill('0');
    escaped << std::hex;
    for (char c : value){
        if (isalnum((unsigned char)c) || c=='-'||c=='_'||c=='.'||c=='~')
            escaped << c;
        else
            escaped << '%' << std::uppercase << std::setw(2)
                    << int((unsigned char)c) << std::nouppercase;
    }
    return escaped.str();
}

// ---------- main ----------
int main(){
    crow::SimpleApp app;

    // root redirect
    CROW_ROUTE(app,"/")([](){
        return crow::response("<meta http-equiv='refresh' content='0;url=/frontend/index.html'>");
    });

    // serve frontend file
    CROW_ROUTE(app,"/frontend/index.html")([](){
        std::ifstream f("frontend/index.html");
        if(!f.is_open()) return crow::response(404,"frontend missing");
        std::stringstream buf; buf<<f.rdbuf();
        return crow::response(buf.str());
    });

    // ---------- geocode (multi-result, fuzzy) ----------
    CROW_ROUTE(app,"/geocode")([](const crow::request& req){
        auto q=req.url_params.get("place");
        if(!q) return crow::response(400,"missing param");
        std::string query=urlEncode(q);
        std::ostringstream u;
        u<<"https://nominatim.openstreetmap.org/search?"
          <<"format=json&addressdetails=1&limit=5&autocomplete=1&q="<<query;

        std::string res=httpGet(u.str());
        json arr=json::parse(res);
        if(arr.empty()) return crow::response("[]");

        // prefer entries that mention "London" or main city if ambiguous
        std::string lowerQ=q; std::transform(lowerQ.begin(),lowerQ.end(),lowerQ.begin(),::tolower);
        json best = arr[0];
        for(auto& cand : arr){
            std::string name = cand["display_name"].get<std::string>();
            std::string lname = name; 
            std::transform(lname.begin(),lname.end(),lname.begin(),::tolower);
            if(lowerQ.find("hyde")!=std::string::npos && lname.find("london")!=std::string::npos)
                best = cand;
        }

        json out = json::array(); out.push_back(best);
        return crow::response(out.dump());
    });

    // ---------- reverse lookup ----------
    CROW_ROUTE(app,"/reverse")([](const crow::request& req){
        auto qs=req.url_params;
        if(!qs.get("lat")||!qs.get("lon"))
            return crow::response(400,"missing coords");
        double lat=std::stod(qs.get("lat"));
        double lon=std::stod(qs.get("lon"));
        std::ostringstream u;
        u<<"https://nominatim.openstreetmap.org/reverse?"
          <<"format=jsonv2&lat="<<lat<<"&lon="<<lon;
        return crow::response(httpGet(u.str()));
    });

    // ---------- route ----------
    CROW_ROUTE(app,"/route")([](const crow::request& req){
        auto qs=req.url_params;
        double slon=std::stod(qs.get("srcLon"));
        double slat=std::stod(qs.get("srcLat"));
        double dlon=std::stod(qs.get("dstLon"));
        double dlat=std::stod(qs.get("dstLat"));
        std::ostringstream u;
        u<<"https://router.project-osrm.org/route/v1/driving/"
          <<slon<<","<<slat<<";"<<dlon<<","<<dlat
          <<"?overview=full&geometries=geojson";
        std::string res=httpGet(u.str());
        json j=json::parse(res);
        json out;
        out["distance_km"]=j["routes"][0]["distance"].get<double>()/1000.0;
        out["duration_min"]=j["routes"][0]["duration"].get<double>()/60.0;
        out["coords"]=j["routes"][0]["geometry"]["coordinates"];
        return crow::response(out.dump());
    });

    // ---------- run + auto-open ----------
    std::thread t([&](){ app.port(8080).multithreaded().run(); });
#ifdef __APPLE__
    system("open http://localhost:8080");
#elif _WIN32
    system("start http://localhost:8080");
#else
    system("xdg-open http://localhost:8080 >/dev/null 2>&1");
#endif
    t.join();
}

