//
// Created by tomu@wincs.cs.bgu.ac.il on 19/11/2019.
//

#include "../include/User.h"
#include "../include/json.hpp"
#include <fstream>
#include "../include/Watchable.h"
#include "../include/Session.h"
#include <sstream>

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
            bool last = (k==seasonsList.size()-1);
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
    command=""; second=""; third="";
    User* DEFAULT = new LengthRecommenderUser("DEFAULT");
    userMap["DEFAULT"] = DEFAULT;
    activeUser = DEFAULT;

}

std::vector<Watchable*> Session::getContent() const {
    return content;
}

User& Session::getActiveUser() const{
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
    std::vector<std::string> possibleActions = {"createuser", "changeuser", "deleteuser", "dupuser", "content", "watch", "log", "watchlist", "exit"};
    std::cout<<"SPLFLIX is now on!"<<std::endl;
    while (command != "exit") {
        command = "";
        second = "";
        third = "";
        std::cout << "what would you like to do? ";
        std::string inputLine;
        getline(std::cin, inputLine);
        std::istringstream iss(inputLine);
        std::getline(iss, command, ' ');
        std::getline(iss, second, ' ');
        std::getline(iss, third, ' ');
        if(!(std::find(possibleActions.begin(), possibleActions.end(), command) != possibleActions.end())) {
            std::cout << "Oops! not a valid commend"<<std::endl;
        }
        else if(command == "createuser"){
            BaseAction* createuser = new CreateUser();
            createuser->act(*this);
            actionsLog.push_back(createuser);
        }
        else if(command == "changeuser"){
            BaseAction* changeuser = new ChangeActiveUser();
            changeuser->act(*this);
            actionsLog.push_back(changeuser);
        }
        else if(command == "deleteuser"){
            BaseAction* deleteuser = new DeleteUser();
            deleteuser->act(*this);
            actionsLog.push_back(deleteuser);
        }
        else if(command == "dupuser"){
            BaseAction* dupuser = new DuplicateUser();
            dupuser->act(*this);
            actionsLog.push_back(dupuser);
        }
        else if(command == "content"){
            BaseAction* cont = new PrintContentList();
            cont->act(*this);
            actionsLog.push_back(cont);
        }
        else if(command == "watch") {
            BaseAction *watch = new Watch();
            watch->act(*this);
            actionsLog.push_back(watch);
            if (watch->getStatus() != ERROR) {
                std::string ans;
                while (ans != "n") {
                    long historySize = activeUser->get_history().size();
                    Watchable *next = activeUser->get_history().at(historySize - 1)->getNextWatchable(*this);
                    if (next == nullptr) {
                        ans = "n";
                        std::cout << "No next recommendations" << std::endl;

                    } else {
                        std::cout << "We recommend watching " + next->toStringHistory() + ", continue watching? [y/n]";
                        getline(std::cin, ans);
                        if (ans != "y" && ans != "n") {
                            std::cout << "please choose y or n" << std::endl;
                        } else if (ans == "y") {
                            second = std::to_string(next->getId());
                            BaseAction *watch2 = new Watch();
                            watch2->act(*this);
                            actionsLog.push_back(watch2);
                        }
                    }
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
    command = "";
    second = "";
    third = "";
    BaseAction* exit = new Exit();
    exit->act(*this);
    actionsLog.push_back(exit);
}

void Session::add_to_user_map(User* user, std::string &name){
    userMap[name] = user;
}

void Session::change_user(User *user) {
    activeUser = user;
}

std::vector<BaseAction*>& Session::getActionsLog(){
    return actionsLog;
}


Session::~Session() {
    clear();
}

void Session::clear(){
    for(Watchable* cont:content){
        delete cont;
    }
    for(BaseAction* action: actionsLog){
        delete action;
    }
    for(std::pair<std::string, User*> us: userMap){
        delete us.second;
    }
}

Session::Session(const Session &other) {
    fillDataStructures(other.content, other.actionsLog, other.userMap);
    for(std::pair<std::string, User*> user: userMap){
        if(other.activeUser->getName() == user.first){
            activeUser = user.second;
        }
    }
}

Session &Session::operator=(const Session &other) {
    if(this != &other){
        clear();
        fillDataStructures(other.content, other.actionsLog, other.userMap);
        for(std::pair<std::string, User*> user: userMap){
            if(other.activeUser->getName() == user.first){
                activeUser = user.second;
            }
        }
    }
    return(*this);
}



void Session::fillDataStructures(const std::vector<Watchable *> &_content, const std::vector<BaseAction *> &_actionLog,
                                 const std::unordered_map<std::string, User *> &_userMap) {
    for(Watchable* cont: _content){
        content.push_back(cont->clone());
    }
    for(BaseAction* action: _actionLog){
        actionsLog.push_back(action->clone());
    }
    for(std::pair<std::string, User*> user: _userMap){
        User* newUser = user.second->clone();
        userMap[user.first] = newUser;
        newUser->set_history({});
        for (Watchable* w : user.second->get_history()){
            for (Watchable* cont : content){
                if (w->toString() == cont->toString()){
                    newUser->addToHistory(cont);
                }
            }
        }
    }
}


Session::Session(const Session&& other){
    *this = other;
    for(Watchable* cont:other.content){
        cont = nullptr;
    }
    for(BaseAction* action: other.actionsLog){
        action = nullptr;
    }
    for(std::pair<std::string, User*> us: other.userMap){
        us.first = "";
        us.second = nullptr;
    }
}


Session& Session::operator=(const Session&& other){
    if(this != &other){
        clear();
        content = other.content;
        actionsLog = other.actionsLog;
        userMap = other.userMap;
        activeUser = other.activeUser;
        for(Watchable* cont:other.content){
            cont = nullptr;
        }
        for(BaseAction* action: other.actionsLog){
            action = nullptr;
        }
        for(std::pair<std::string, User*> us: other.userMap){
            us.first = "";
            us.second = nullptr;
        }
    }
    return *this;
}
