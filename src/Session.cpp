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
Session::Session(const std::string &configFilePath):command(""), second(""),third(""),content({}), actionsLog({}), userMap({}), activeUser(nullptr){
    std::ifstream i(configFilePath);
    json j;
    i >> j;
    json movies = j["movies"];
    unsigned long id = 1;
    for(json& movie: movies) {
        Movie* tmpMovie = new Movie(id, movie["name"], movie["length"], movie["tags"]);
        content.push_back(tmpMovie);
        id++;
    }
    json series = j["tv_series"];
    for (json& tmp_series: series) {
        json seasonsList = tmp_series.at("seasons");
        for (unsigned int k = 0; k < seasonsList.size(); k++) {
            bool last = (k==seasonsList.size()-1);
            unsigned long episodesNumber = tmp_series.at("seasons")[k];
            for (unsigned long e = 1; e <= episodesNumber; e++) {
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
    command = "";
    second = "";
    third = "";
    User* DEFAULT = new LengthRecommenderUser("DEFAULT");
    userMap["DEFAULT"] = DEFAULT;
    activeUser = DEFAULT;

}

std::vector<BaseAction*> Session::getActionsLog() const{
    return actionsLog;
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
    std::vector<std::string> possibleActions = {"createuser", "changeuser", "deleteuser", "dupuser", "content", "watch", "log", "watchhist", "exit"};
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
                        std::cout << "We recommend watching " + next->toString() + ", continue watching? [y/n]";
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
        else if (command == "watchhist"){
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
    actionsLog.push_back(exit);
}

void Session::add_to_user_map(User* user, std::string &name){
    userMap[name] = user;
}

void Session::change_user(User *user) {
    activeUser = user;
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

Session::Session(const Session &other):command(""), second(""),third(""),content({}), actionsLog({}), userMap({}), activeUser(nullptr) {
    fillDataStructures(other.content, other.actionsLog, other.userMap);
    for(std::pair<std::string, User*> user: userMap){
        if(other.activeUser->getName() == user.first){
            activeUser = user.second;
        }
    }
}

Session& Session::operator=(const Session &other) {
    if(this != &other){
        clear();
        clean();
        fillDataStructures(other.content, other.actionsLog, other.userMap);
        for(std::pair<std::string, User*> user: userMap){
            if(other.activeUser->getName() == user.first){
                activeUser = user.second;
            }
        }
    }
    return(*this);
}



void Session::fillDataStructures(const std::vector<Watchable *> &_content, const std::vector<BaseAction *> &_actionLog, const std::unordered_map<std::string, User *> &_userMap) {
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
                if (w->fullToString() == cont->fullToString()){
                    newUser->addToHistory(cont);
                }
            }
        }
    }
}


Session::Session(Session&& other):command(""), second(""),third(""),content({}), actionsLog({}), userMap({}), activeUser(nullptr){
    std::string activeUSerName = other.activeUser->getName();
    stealDataStructures(other.content, other.actionsLog, other.userMap);
    for(std::pair<std::string, User*> user: userMap){
        if(activeUSerName == user.first){
            activeUser = user.second;
        }
    }
}


Session& Session::operator=(Session&& other){
    if(this != &other){
        clear();
        clean();
        stealDataStructures(other.content, other.actionsLog, other.userMap);
        activeUser = other.activeUser;
        other.activeUser = nullptr;
    }
    return *this;
}

void Session::stealDataStructures(std::vector<Watchable *> &_content, std::vector<BaseAction *> &_actionLog, std::unordered_map<std::string, User *> &_userMap) {
    for(unsigned int i = 0; i < _content.size(); i++){
        content.push_back(_content.at(i));
    }
    for(unsigned int i = 0; i < _actionLog.size(); i++){
        actionsLog.push_back(_actionLog.at(i));
        _actionLog.at(i) = nullptr;
    }
    for(std::pair<std::string, User*> user: _userMap){
        User* newUser = user.second;
        userMap[user.first] = newUser;
        newUser->set_history({});
        for (Watchable* w : user.second->get_history()){
            for (Watchable* cont : content){
                if (w->toString() == cont->toString()){
                    newUser->addToHistory(cont);
                }
            }
        }
        user.first = "";
        _userMap[user.first] = nullptr;
    }
    for(unsigned int i = 0; i < _content.size(); i++){
        _content.at(i) = nullptr;
    }
}

void Session::clean() {
    content.clear();
    actionsLog.clear();
    userMap.clear();
    command.clear();
    second.clear();
    third.clear();
}

