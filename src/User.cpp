#include "../include/Watchable.h"
#include "../include/User.h"


User::User(const std::string& _name):name(_name){

}

std::string User::getName() const {
    return name;
}

std::vector<Watchable*> User::get_history() const{
    return history;
}

LengthRecommenderUser::LengthRecommenderUser(const std::string& _name):User(_name){

}


Watchable* LengthRecommenderUser::getRecommendation(Session& s){
    /* int sum = 0;
    for(Watchable& watched: s.get_active_user->get_history()){
        sum += watched.getLength();
    }
    int desiredLength = average / s.get_active_user->get_history().size(); */
    return nullptr;
}

RerunRecommenderUser::RerunRecommenderUser(const std::string& _name):User(_name){
}


Watchable* RerunRecommenderUser::getRecommendation(Session& s){
    return nullptr;
}
GenreRecommenderUser::GenreRecommenderUser(const std::string& _name):User(_name){
}
Watchable* GenreRecommenderUser::getRecommendation(Session& s){
    return nullptr;
}


