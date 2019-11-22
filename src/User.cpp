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

void User::setRecommendedAlgorithm(std::string s)  {
    recommendedAlgorithm = s;
}

std::vector<Watchable*> User::get_history() const{
    return history;
}

void User::set_history(std::vector<Watchable *> _history) {
    history = _history;
}

void User::addToHistory(Watchable *w) {
    history.push_back(w);
}

LengthRecommenderUser::LengthRecommenderUser(const std::string& _name): User(_name) {
    setRecommendedAlgorithm("len");
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
    setRecommendedAlgorithm("rer");
    if (!history.empty()) {
        lastRecommenedWatchable = get_history().at(0);
    }
}

Watchable* RerunRecommenderUser::getRecommendation(Session& s) {
    Watchable *returnedShow = nullptr;
    bool foundShow = false;
    int historySize = s.get_active_user()->get_history().size();
    for (int i = 0; !foundShow && i < historySize; i++) {
        if (std::find(history.begin(), history.end(), lastRecommenedWatchable) != history.end()) {
            returnedShow = s.get_active_user()->get_history().at((i + 1) % historySize);
            foundShow = true;
            lastRecommenedWatchable = returnedShow;
        }

    }
    return returnedShow;
}



GenreRecommenderUser::GenreRecommenderUser(const std::string& _name):User(_name){
    setRecommendedAlgorithm("gen");

}
Watchable* GenreRecommenderUser::getRecommendation(Session& s){
    int noContentCounter = 0;
    Watchable* returnedShow = nullptr;
    sort(s.get_active_user()->getPopularTags().begin(), s.get_active_user()->getPopularTags().end());
    int popularTagsSize = s.get_active_user()->getPopularTags().size();
    auto mostPopularTag = s.get_active_user()->getPopularTags().at(popularTagsSize - 1);
    std::vector<Watchable*> history = s.get_active_user()->get_history();
    for(Watchable* cont: s.getContent()){
        std::vector<std::string> currentContTags = cont->getTags();
        if(!(std::find(history.begin(), history.end(), cont) != history.end())){
            for(std::string tag: currentContTags) {
                if(tag == mostPopularTag.first) {
                    returnedShow = cont;
                }
                else {
                    noContentCounter++;
                    mostPopularTag = s.get_active_user()->getPopularTags().at(popularTagsSize - 1 - noContentCounter);
                }
            }
        }
    }
    return returnedShow;

}

std::vector<std::pair<std::string,long>>& User::getPopularTags(){
    return popularTags;
}

void User::increaseTag(std::string &tag) {
    if(std::find(history.begin(), history.end(), tag) != history.end()){

    }
}

