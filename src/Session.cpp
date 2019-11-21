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
    last_input="";
    User* DEFAULT = new LengthRecommenderUser("Default");
    userMap["Default"] = DEFAULT;

}

std::vector<Watchable*> Session::getContent() const {
    return content;
}

User* Session::get_active_user() const{
    return activeUser;
}

std::string Session::get_last_input(){
    return last_input;
}


void Session::start() {
    printf("SPLFLIX is now on!”");
    while (last_input != "exit") {
        std::cout << "\nwhat would you like to do? ";
        std::cin >> last_input;
    }
    BaseAction* exit = new Exit();
    exit->act(*this);


}

void Session::add_user(User* user, std::string name){
    userMap[name] = user;
}


Session::~Session() {

}