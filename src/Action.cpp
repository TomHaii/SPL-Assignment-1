//
// Created by yaelgeis@wincs.cs.bgu.ac.il on 20/11/2019.
//
#include "../include/User.h"
#include "../include/json.hpp"
#include <fstream>
#include "../include/Watchable.h"
#include "../include/Session.h"



BaseAction::BaseAction(){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete() {}

void CreateUser::act(Session& sess){
    std::string input = sess.get_last_input();
    int len = input.length();
    std::string user_name; std::string algorithm;
    user_name = input.substr(11, len-15);
    algorithm = input.substr(len-3, 3);
    User* new_user;
    if (algorithm == "len"){
        new_user = new LengthRecommenderUser(user_name);
    }
    else if (algorithm =="rer"){
        new_user = new RerunRecommenderUser(user_name);
    }
    else{
        new_user = new GenreRecommenderUser(user_name);
    }
    sess.add_user(new_user,user_name);
    complete();
}

std::string CreateUser::toString() const {
    if (getStatus() == COMPLETED){
        return "CreateUser COMPLETED";
    }
    else if (getStatus() == ERROR){

    }
}