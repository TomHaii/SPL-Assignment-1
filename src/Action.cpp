//
// Created by yaelgeis@wincs.cs.bgu.ac.il on 20/11/2019.
//
#include "../include/User.h"
#include "../include/json.hpp"
#include <fstream>
#include "../include/Watchable.h"
#include "../include/Session.h"
#include <unordered_map>



BaseAction::BaseAction(){
    status = PENDING;
}

ActionStatus BaseAction::getStatus() const{
    return status;
}


void BaseAction::error(const std::string &_errorMsg) {
    status = ERROR;
    errorMsg = _errorMsg;
    std::cout<<errorMsg<<std::endl;
}

void BaseAction::complete() {
    status = COMPLETED;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void CreateUser::act(Session& sess){
    std::string user_name = sess.secondInput();
    std::string algorithm = sess.thirdInput();
    if (sess.getUserMap().count(user_name) > 0) { //username taken
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
    if (getStatus() == COMPLETED){
        return "CreateUser COMPLETED";
    }
    else if (getStatus() == ERROR){
        return ("CreateUser ERROR: " + getErrorMsg());
    }
    return "";
}

void ChangeActiveUser::act(Session &sess) {
    std::string user_name = sess.secondInput();
    std::unordered_map<std::string, User *> map = sess.getUserMap();
    if (map.count(user_name) == 0) {
        error("no such user");
    } else {
        sess.change_user(sess.getUserMap()[user_name]);
        complete();
    }
}


std::string ChangeActiveUser::toString() const {
    if (getStatus() == COMPLETED){
        return "ChangeUser COMPLETED";
    }
    else if (getStatus() == ERROR){
        return ("ChangeUser ERROR: " + getErrorMsg());
    }
    return "";
}

void DeleteUser::act(Session &sess) {
    std::string user_name = sess.secondInput();
    std::unordered_map<std::string, User *> map = sess.getUserMap();
    if (map.count(user_name) == 0) {
        error("no such user");
    } else if (sess.get_active_user().getName() == user_name) {
        sess.change_user(map["DEFAULT"]);
    }
        sess.erase_user(user_name);
        complete();
}

std::string DeleteUser::toString() const {
    if (getStatus() == COMPLETED){
        return "DeleteUser COMPLETED";
    }
    else if (getStatus() == ERROR){
        return ("DeleteUser ERROR: " + getErrorMsg());
    }
    return "";
}

void DuplicateUser::act(Session &sess) {
    std::string orig_user = sess.secondInput();
    std::string new_user = sess.thirdInput();
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
    if (getStatus() == COMPLETED){
        return "DuplicateUser COMPLETED";
    }
    return ("DuplicateUser ERROR: " + getErrorMsg());
}

void PrintContentList::act(Session &sess){
    for(Watchable* w : sess.getContent()){
        std::cout << w->toString() <<std::endl;
    }
    complete();
}

std::string PrintContentList::toString() const {
    return "PrintContentList COMPLETED";
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
    return "PrintWatchHistory COMPLETED";
}

void Watch::act(Session &sess) {
    std::string _id = sess.secondInput();;
    long id = std::stol(_id);
    if (id > sess.getContent().size()) {
        error("invalid id inserted");
    } else {
        Watchable* w = sess.getContent().at(id - 1);
        std::cout << "Watching " + w->toStringHistory()<<std::endl;
        sess.get_active_user().addToHistory(w);
        complete();
    }
}

std::string Watch::toString() const {
    if (getStatus() == ERROR){
        return +"Watch ERROR: " +getErrorMsg();
    }
    return "Watch COMPLETED";
}

void PrintActionsLog::act(Session &sess) {
    std::vector<BaseAction*>& log = sess.getActionsLog();
    for (BaseAction* action : log){
            std::cout<<action->toString()<<std::endl;
    }
}

std::string PrintActionsLog::toString() const {
     return "PrintActionsLog COMPLETED";
}

void Exit::act(Session& sess) {
    std::cout <<"LEAVING SPLFLIX\nSee you next time!";
    complete();
}

std::string Exit::toString() const {
    return "Exit COMPLETED";
}




