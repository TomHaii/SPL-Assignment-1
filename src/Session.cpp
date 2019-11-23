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
    long id = 1;
    for(json& movie: movies) {
        Movie* tmpMovie = new Movie(id, movie["name"], movie["length"], movie["tags"]);
        content.push_back(tmpMovie);
        id++;
    }
    json series = j["tv_series"];
    for (json& tmp_series: series) {
        json seasonsList = tmp_series.at("seasons");
        for (int k = 0; k < seasonsList.size(); k++) {
            bool last = (k==seasonsList.size());
            long episodesNumber = tmp_series.at("seasons")[k];
            for (long e = 1; e <= episodesNumber; e++) {
                Episode *tmpEpisode = new Episode(id, tmp_series["name"], tmp_series["episode_length"], k + 1, e, tmp_series["tags"]);
                if (!last || (e+1 <= episodesNumber)){
                    tmpEpisode->setNextEpisode(id);
                }
                else{
                    tmpEpisode->setNextEpisode(-1);
                }
                content.push_back(tmpEpisode);
                id++;
            }
        }
    }
    last = nullptr;
    command=""; second=""; third="";
    User* DEFAULT = new LengthRecommenderUser("DEFAULT");
    userMap["DEFAULT"] = DEFAULT;
    activeUser = DEFAULT;
    possibleActions = {"createuser", "changeuser", "deleteuser", "dupuser", "content", "watch", "log", "watchlist", "exit"};
}

std::vector<Watchable*> Session::getContent() const {
    return content;
}

User& Session::get_active_user(){
    return *activeUser;
}

std::string Session::secondInput() const {
    return second;
}

std::string Session::thirdInput() const {
    return third;
}

void Session::erase_user(std::string name) {
    delete(userMap[name]);
    userMap.erase(name);
}


std::unordered_map<std::string,User*> Session::getUserMap() const{
    return userMap;
}


void Session::start() {
    std::cout<<"SPLFLIX is now on!"<<std::endl;
    while (command != "exit") {
        std::cout << "what would you like to do? ";
        std::cin >> command;
        if(!(std::find(possibleActions.begin(), possibleActions.end(), command) != possibleActions.end())) {
            std::cout << "Oops! not a valid commend"<<std::endl;
        }
        else if(command == "createuser"){
            std::cin >> second;
            std::cin >> third;
            BaseAction* createuser = new CreateUser();
            createuser->act(*this);
            actionsLog.push_back(createuser);
        }
        else if(command == "changeuser"){
            std::cin >> second;
            BaseAction* changeuser = new ChangeActiveUser();
            changeuser->act(*this);
            actionsLog.push_back(changeuser);
        }
        else if(command == "deleteuser"){
            std::cin >> second;
            BaseAction* deleteuser = new DeleteUser();
            deleteuser->act(*this);
            actionsLog.push_back(deleteuser);
        }
        else if(command == "dupuser"){
            std::cin >> second;
            std::cin >> third;
            BaseAction* dupuser = new DuplicateUser();
            dupuser->act(*this);
            actionsLog.push_back(dupuser);
        }
        else if(command == "content"){
            BaseAction* content = new PrintContentList();
            content->act(*this);
            actionsLog.push_back(content);
        }
        else if(command == "watch"){
            std::cin >> second;
            BaseAction* watch = new Watch();
            watch->act(*this);
            actionsLog.push_back(watch);
            std::string ans="";
            while (ans != "n"){
                long historySize = activeUser->get_history().size();
                Watchable* next = activeUser->get_history().at(historySize-1)->getNextWatchable(*this);
                std::cout<<"We recommend watching " + next->toStringHistory()+", continue watching? [y/n]"<<std::endl;
                std::cin >> ans;
                if (ans != "y" && ans != "n"){
                    std::cout<<"please choose y or n"<<std::endl;
                }
                else if (ans == "y") {
                    second = std::to_string(next->getId());
                    BaseAction* watch2 = new Watch();
                    watch2->act(*this);
                    actionsLog.push_back(watch2);
                }
            }
        }
        else if (command == "watchlist"){
            BaseAction* watchlist = new PrintWatchHistory;
            watchlist->act(*this);
            actionsLog.push_back(watchlist);
        }
        else if(command == "log"){
            BaseAction* log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);
        }
    }
    BaseAction* exit = new Exit();
    exit->act(*this);
}

void Session::add_to_user_map(User* user, std::string name){
    userMap[name] = user;
}

void Session::change_user(User *user) {
    activeUser = user;
}

std::vector<BaseAction*>& Session::getActionsLog(){
    return actionsLog;
}


Session::~Session() {

}