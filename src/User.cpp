#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"

User::User(const std::string& _name):name(_name), recommendedAlgorithm("len"){}

std::string User::getName() const {
    return name;
}

std::vector<Watchable*> User::get_history() const{
    return history;
}

LengthRecommenderUser::LengthRecommenderUser(const std::string& _name):User(_name){

}


Watchable* LengthRecommenderUser::getRecommendation(Session& s){
    unsigned int sum = 0;
    std::vector<Watchable*> history = s.get_active_user()->get_history();
    for(Watchable *watched: history){
        sum += watched->getLength();
    }
    unsigned int desiredLength = sum / history.size();
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


