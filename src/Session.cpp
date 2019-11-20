//
// Created by tomu@wincs.cs.bgu.ac.il on 19/11/2019.
//

#include "../include/User.h"
#include "../include/json.hpp"
#include <fstream>
#include "../include/Watchable.h"
#include "../include/Session.h"

using json = nlohmann::json;
Session::Session(const std::string &configFilePath) {
    std::ifstream i(configFilePath);
    json j;
    i >> j;
    json movies = j.at("movies");
    long id = 0;
    for(json& val: movies) {
        id++;
        Watchable *tmpMovie = new Movie(id, val["name"], val["length"], val["tags"]);
        content.push_back(tmpMovie);
    }
    json series = j.at("tv_series");
    for (json& val: series) {
        json seasonsList = val.at("seasons");
        for (int k = 0; k < seasonsList.size(); k++) {
            int episodesNumber = val.at("seasons")[k];
            for (int e = 1; e <= episodesNumber; e++) {
                id++;
                Watchable *tmpEpisode = new Episode(id, val["name"], val["episode_length"], k+1, e, val["tags"]);
                content.push_back(tmpEpisode);
            }
        }
    }
   // User* DEFAULT = new LengthRecommenderUser("Default");
   // userMap["Default"] = DEFAULT;


}

User* Session::get_active_user() const{
    return activeUser;
}


void Session::start() {
    printf("SPLFLIX is now on!”");

}



Session::~Session() {

}