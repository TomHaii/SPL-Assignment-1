#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"
#include <algorithm>


User::User(const std::string& _name):name(_name), recommendedAlgorithm("len"){
    history;
}

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

void User::set_history(std::vector<Watchable*> _history) {
    history = _history;
}

void User::addToHistory(Watchable* w) {
    history.push_back(w);
}

LengthRecommenderUser::LengthRecommenderUser(const std::string& _name): User(_name) {
    setRecommendedAlgorithm("len");
}

Watchable* LengthRecommenderUser::getRecommendation(Session& s){
    unsigned int sum = 0;
    Watchable* returnedShow = nullptr;
    std::vector<Watchable*> history = s.get_active_user()->get_history();
    for(Watchable*& watched: history){
        sum = sum + watched->getLength();
    }
    int desiredLength = sum / history.size();
    int bestDifference = std::numeric_limits<int>::max();
    for(Watchable* cont: s.getContent()){
        int difference = abs(desiredLength - cont->getLength());
        if((!(std::find(history.begin(), history.end(), cont) != history.end())) && difference < bestDifference) {
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

bool User::compareTagsPairs(const std::pair<std::string,long> &pair1,const std::pair<std::string,long> &pair2) {
    return pair2.second > pair1.second;
}

GenreRecommenderUser::GenreRecommenderUser(const std::string& _name):User(_name){
    setRecommendedAlgorithm("gen");

}
Watchable* GenreRecommenderUser::getRecommendation(Session& s){
    int noContentCounter = 0;
    Watchable* returnedShow = nullptr;
    bool foundShow = false;
    sort(s.get_active_user()->getPopularTags().begin(), s.get_active_user()->getPopularTags().end(), compareTagsPairs);
    int popularTagsSize = s.get_active_user()->getPopularTags().size();
    auto mostPopularTag = s.get_active_user()->getPopularTags().at(popularTagsSize - 1);
    std::vector<Watchable*> history = s.get_active_user()->get_history();
    std::cout << "checheck" << std::endl;

    for(Watchable*& cont: s.getContent()){
        if(!(std::find(history.begin(), history.end(), cont) != history.end())){
            std::cout << "checheck2" << std::endl;

            std::cout << "Test: Didn't watch " + cont->getTags()[0] << std::endl;

            std::vector<std::string> currentContTags = cont->getTags();
            for(std::string &tag: currentContTags) {
                if(tag == mostPopularTag.first) {
                    std::cout << "chechec3" << std::endl;
                    returnedShow = cont;
                    foundShow = true;
                }
            }
        }
        // NEED TO FIX THAT SHIT
//        if(!foundShow){
//            noContentCounter++;
//            mostPopularTag = s.get_active_user()->getPopularTags().at(popularTagsSize - 1 - noContentCounter);
//        }
    }
    return returnedShow;

}

std::vector<std::pair<std::string,long>>& User::getPopularTags(){
    return popularTags;
}

void User::increaseTag(std::string &tag) {
    int index = 0;
    bool tagNotFound = true;
    for(std::pair<std::string, long>& p: popularTags){
        if(p.first == tag){
            p.second++;
            tagNotFound = false;
        }
        index++;
    }
    if(tagNotFound){
        std::cout << "Added Tag" + tag << std::endl;
        popularTags.emplace_back(tag,1);
    }
}

