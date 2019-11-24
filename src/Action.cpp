//
// Created by yaelgeis@wincs.cs.bgu.ac.il on 20/11/2019.
//
#include "../include/User.h"
#include "../include/json.hpp"
#include <fstream>
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/Action.h"

#include <unordered_map>


BaseAction::BaseAction(){
    errorMsg = "";
    status = PENDING;
}

ActionStatus BaseAction::getStatus() const{
    return status;
}


void BaseAction::error(const std::string &_errorMsg) {
    status = ERROR;
    errorMsg.assign(_errorMsg);
    std::cout << errorMsg << std::endl;
}

void BaseAction::complete() {
    status = COMPLETED;
}

std::string BaseAction::getErrorMsg() const {
    std::cout<<errorMsg<<std::endl;
    return errorMsg;
}

std::string BaseAction::getError() const {
    return errorMsg;
}

void CreateUser::act(Session& sess){
    std::string user_name = sess.secondInput();
    std::string algorithm = sess.thirdInput();
    if (user_name.empty() || algorithm.empty()){
        error("invalid input");
    }
    else if (sess.getUserMap().count(user_name) > 0) { //username taken
        error("the user name inserted is already taken");
    } else if (algorithm != "len" & algorithm != "rer" & algorithm != "gen") { //not an algorithm
        error("invalid algorithm");
    } else {
        User *new_user;
        if (algorithm == "len") {
            new_user = new LengthRecommenderUser(user_name);
        } else if (algorithm == "rer") {
            new_user = new RerunRecommenderUser(user_name);
        } else {
            new_user = new GenreRecommenderUser(user_name);
        }
        sess.add_to_user_map(new_user, user_name);
        complete();
        }
}

std::string CreateUser::toString() const {
    return "CreateUser";
}

void ChangeActiveUser::act(Session &sess) {
    std::string user_name = sess.secondInput();
    if (user_name.empty()){
        error("invalid input");
    }
    else {
        std::unordered_map<std::string, User *> map = sess.getUserMap();
        if (map.count(user_name) == 0) {
            error("no such user");
        } else {
            sess.change_user(sess.getUserMap()[user_name]);
            complete();
        }
    }
}


std::string ChangeActiveUser::toString() const {
    return "ChangeUser";
}


void DeleteUser::act(Session &sess) {
    std::string user_name = sess.secondInput();
    if (user_name.empty()){
        error("invalid input");
    }
    else {
        std::unordered_map<std::string, User *> map = sess.getUserMap();
        if (map.count(user_name) == 0) {
            error("no such user");
        } else if (sess.get_active_user().getName() == user_name) {
            sess.change_user(map["DEFAULT"]);
        }
        sess.erase_user(user_name);
        complete();
    }
}

std::string DeleteUser::toString() const {
    return "DeleteUser";
}

void DuplicateUser::act(Session &sess) {
    std::string orig_user = sess.secondInput();
    std::string new_user = sess.thirdInput();
    if (orig_user.empty() || new_user.empty()){
        error("invalid input");
    }
    std::unordered_map<std::string, User *> map = sess.getUserMap();
    if (map.count(orig_user) == 0) {
        error("no such user");
    } else {
        if (map.count(new_user) > 0) {
            error("the new user name is already taken");
        } else {
            User *curr = map[orig_user];
            User *duplicated;
            if (curr->getRecommendedAlgorithm() == "len") {
                duplicated = new LengthRecommenderUser(new_user);
            } else if (curr->getRecommendedAlgorithm() == "rer") {
                duplicated = new RerunRecommenderUser(new_user);
            } else {
                duplicated = new GenreRecommenderUser(new_user);
            }
            duplicated->set_history(curr->get_history());
            sess.add_to_user_map(duplicated, new_user);
            complete();
        }
    }
}

std::string DuplicateUser::toString() const {
    return "DuplicateUser";
}

void PrintContentList::act(Session &sess){
    for(Watchable* w : sess.getContent()){
        std::cout << w->toString() <<std::endl;
    }
    complete();
}

std::string PrintContentList::toString() const {
    return "PrintContentList";
}

void PrintWatchHistory::act(Session &sess){
    std::string output = "watch history for " + sess.get_active_user().getName() + ":\n";
    std::cout<<"watch history for "+ sess.get_active_user().getName() + ":"<<std::endl;
    int i = 1;
    for(Watchable* w : sess.get_active_user().get_history()) {
        std::cout << std::to_string(i) + ". " + w->toStringHistory() << std::endl;
        i++;
    }
    complete();
}

std::string PrintWatchHistory::toString() const {
    return "PrintWatchHistory";
}

void Watch::act(Session &sess) {
    std::string _id = sess.secondInput();
    bool isNumber = false;
    long id = -1;
    std::string::const_iterator it = _id.begin();
    while (it != _id.end() && std::isdigit(*it)) {
        it++;
    }
    isNumber = !_id.empty() && it == _id.end();
    if (_id.empty() || !isNumber) {
        error("invalid input");
    }
    else {
        id = std::stol(_id);
        if (id > sess.getContent().size()) {
            error("invalid id inserted");
        } else {
            Watchable *w = sess.getContent().at(id - 1);
            std::cout << "Watching " + w->toStringHistory() << std::endl;
            sess.get_active_user().addToHistory(w);
            complete();
        }
    }
}


std::string Watch::toString() const {
    return "Watch";
}

void PrintActionsLog::act(Session &sess) {
    std::vector<BaseAction*>& log = sess.getActionsLog();
    for (long i = (long) log.size()-1; i >= 0; i--){
        BaseAction* action = log.at(i);
        if (action->getStatus() == COMPLETED){
            std::cout <<action->toString() +" COMPLETED" << std::endl;
        }
        else{
            std::cout << action->toString() + " ERROR: " + action->getError() << std::endl;
        }

    }
}

std::string PrintActionsLog::toString() const {
     return "PrintActionsLog";
}

void Exit::act(Session& sess) {
    std::cout <<"LEAVING SPLFLIX"<<std::endl;
    std::cout <<"See you next time!"<<std::endl;
    complete();
}

std::string Exit::toString() const {
    return "Exit";
}


