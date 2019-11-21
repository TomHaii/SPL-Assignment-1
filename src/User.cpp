#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"
#include <algorithm>


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
    int sum = 0;
    Watchable* returnedShow = nullptr;
    std::vector<Watchable*> history = s.get_active_user()->get_history();
    for(Watchable *watched: history){
        sum += watched->getLength();
    }
    int desiredLength = sum / history.size();
    int bestDifference = std::numeric_limits<int>::max();
    for(Watchable* cont: s.getContent()){
        int difference = abs(desiredLength - cont->getLength());
        if(!(std::find(history.begin(), history.end(), cont) != history.end() && difference > bestDifference)) {
            bestDifference = difference;
            returnedShow = cont;
        }
    }
    return returnedShow;
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


