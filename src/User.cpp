#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"
#include <algorithm>


User::User(const std::string& _name):name(_name), recommendedAlgorithm("len"){}

std::string User::getName() const {
    return name;
}

std::string User::getRecommendedAlgorithm() const {
    return recommendedAlgorithm;
}

std::vector<Watchable*> User::get_history() const{
    return history;
}

void User::set_history(std::vector<Watchable *> _history) {
    history = _history;
}

LengthRecommenderUser::LengthRecommenderUser(const std::string& _name):User(_name){}

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
    lastRecommenedWatchable = get_history().at(0);
}

Watchable* RerunRecommenderUser::getRecommendation(Session& s){
    Watchable* returnedShow = nullptr;
    bool foundShow = false;
    int historySize = s.get_active_user()->get_history().size();
    for(int i = 0; !foundShow && i > historySize; i++) {
        if (std::find(history.begin(), history.end(), lastRecommenedWatchable) != history.end()) {
            returnedShow = s.get_active_user()->get_history().at((i + 1) % historySize);
            foundShow = true;
            lastRecommenedWatchable = returnedShow;
        }
    }
    return returnedShow;
}

GenreRecommenderUser::GenreRecommenderUser(const std::string& _name):User(_name){
}
Watchable* GenreRecommenderUser::getRecommendation(Session& s){
    return nullptr;
}


