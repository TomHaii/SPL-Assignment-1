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
    json movies = j["movies"];
    long id = 0;
    for(json& movie: movies) {
        id++;
        Watchable *tmpMovie = new Movie(id, movie["name"], movie["length"], movie["tags"]);
        content.push_back(tmpMovie);
    }
    json series = j["tv_series"];
    for (json& tmp_series: series) {
        json seasonsList = tmp_series.at("seasons");
        for (int k = 0; k < seasonsList.size(); k++) {
            int episodesNumber = tmp_series.at("seasons")[k];
            for (int e = 1; e <= episodesNumber; e++) {
                id++;
                Watchable *tmpEpisode = new Episode(id, tmp_series["name"], tmp_series["episode_length"], k + 1, e, tmp_series["tags"]);
                content.push_back(tmpEpisode);
            }
        }
    }
    last_input="";
    User* DEFAULT = new LengthRecommenderUser("DEFAULT");
    userMap["DEFAULT"] = DEFAULT;

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

void Session::erase_user(std::string name) {
    delete(userMap[name]);
    userMap.erase(name);
}


std::unordered_map<std::string,User*> Session::getUserMap() const{
    return userMap;
}



void Session::start() {
    printf("SPLFLIX is now on!”");
    while (last_input != "exit") {
        std::cout << "\nwhat would you like to do? ";
        std::cin >> last_input;
    }
//    BaseAction* exit = new Exit();
//    exit->act(*this);
}

void Session::add_to_user_map(User* user, std::string name){
    userMap[name] = user;
}

void Session::change_user(User *user) {
    activeUser = user;
}


Session::~Session() {

}