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
}

void BaseAction::complete() {
    status = COMPLETED;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void CreateUser::act(Session& sess){
    std::string input = sess.get_last_input();
    int len = input.length();
    if (len < 16){ //not long enough
        error("invalid input");
    }
    if (getStatus() != ERROR) {
        std::string user_name;
        std::string algorithm;
        user_name = input.substr(11, len - 15);
        algorithm = input.substr(len - 3, 3);
        std::unordered_map<std::string,User*> map = sess.getUserMap();
        if (map.count(user_name) > 0) { //username taken
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
    std::string input = sess.get_last_input();
    int len = input.length();
    if (len < 12){
        error("invalid input");
    }
    if (getStatus() != ERROR) {
        std::string user_name;
        user_name = input.substr(11, len - 11);
        std::unordered_map<std::string,User*> map = sess.getUserMap();
        if (map.count(user_name) == 0) {
            error("no such user");
        }
        else{
            sess.change_user(sess.getUserMap()[user_name]);
            complete();
        }
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
    std::string input = sess.get_last_input();
    int len = input.length();
    if (len < 12){
        error("invalid input");
    }
    if (getStatus() != ERROR) {
        std::string user_name;
        user_name = input.substr(11, len - 11);
        std::unordered_map<std::string,User*> map = sess.getUserMap();
        if (map.count(user_name) == 0) {
            error("no such user");
        }
        else{
            sess.change_user(map["DEFAULT"]);
            sess.erase_user(user_name);
            complete();
        }
    }
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
    std::string input = sess.get_last_input();
    int len = input.length();
    if (len < 11){
        error("invalid input");
    }
    if (getStatus() != ERROR) {
        std::string orig_user;  std::string new_user;
        std::string users = input.substr(11, len - 11);
        int i = 0;
        while(users.at(i) != ' '){
            i++;
        }
        orig_user =  users.substr(0, i-1);
        new_user = users.substr(i+1, users.length()-1-i);
        std::unordered_map<std::string,User*> map = sess.getUserMap();
        if (map.count(orig_user) == 0) {
            error("no such user");
        }
        else{
            if (map.count(new_user) > 0){
                error("the new user name is already taken");
            }
            else {
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
}

std::string DuplicateUser::toString() const {
    if (getStatus() == COMPLETED){
        return "DuplicateUser COMPLETED";
    }
    else if (getStatus() == ERROR){
        return ("DuplicateUser ERROR: " + getErrorMsg());
    }
    return "";
}



