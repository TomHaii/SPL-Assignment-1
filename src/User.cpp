#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"
#include <algorithm>
#include <vector>


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

void User::set_history(std::vector<Watchable*> _history) {
    history = std::move(_history);
}


LengthRecommenderUser::LengthRecommenderUser(const std::string& _name): User(_name) {
    setRecommendedAlgorithm("len");
}

Watchable* LengthRecommenderUser::getRecommendation(Session& s){
    Watchable* returnedShow = nullptr;
    int bestDifference = std::numeric_limits<int>::max();
    for(Watchable* cont: s.getContent()){
        int difference = abs(average - cont->getLength());
        if((!(std::find(history.begin(), history.end(), cont) != history.end())) && difference < bestDifference) {
            bestDifference = difference;
            returnedShow = cont;
        }
    }
    return returnedShow;
}

void LengthRecommenderUser::addToHistory(Watchable* w) {
    long sum = average*history.size();
    sum += w->getLength();
    average = sum/(history.size()+1);
    history.push_back(w);
}



RerunRecommenderUser::RerunRecommenderUser(const std::string& _name):User(_name){
    setRecommendedAlgorithm("rer");
    lastRecommendation = -1;
}

Watchable* RerunRecommenderUser::getRecommendation(Session& s) {
    lastRecommendation = (lastRecommendation +1 ) % (long)(history.size());
    return history.at(lastRecommendation);
}

void RerunRecommenderUser::addToHistory(Watchable* w) {
    history.push_back(w);
}

GenreRecommenderUser::GenreRecommenderUser(const std::string& _name):User(_name){
    setRecommendedAlgorithm("gen");
    mostPopularTag.first="";
    mostPopularTag.second=-1;
}

Watchable* GenreRecommenderUser::getRecommendation(Session& s) {
    std::vector<std::string> prevTags;
    std::string wantedTag = mostPopularTag.first;
    Watchable *next = nullptr;
    long i = 1;
    while (!wantedTag.empty()) {
        i++;
        for (Watchable *&w : s.getContent()) {
            std::vector<std::string> tags = w->getTags();
            if ((std::find(tags.begin(), tags.end(), wantedTag) != tags.end() &&
                 (!(std::find(history.begin(), history.end(), *&w) != history.end())))) {
                return w;
            }
        }
        prevTags.push_back(wantedTag);
        wantedTag = getNextPopular(prevTags);
    }
    return next;

}

void GenreRecommenderUser::addToHistory(Watchable* w) {
    history.push_back(w);
    for (std::string &tag : w->getTags()) {
        tagsMap[tag]++;
        if (tagsMap.count(tag) == 0){
            tagsMap[tag] = 1;
        }
        if ((tagsMap[tag] > mostPopularTag.second) || (tagsMap[tag] == mostPopularTag.second && tag.compare(mostPopularTag.first)<0)) {
            mostPopularTag.first = tag;
            mostPopularTag.second = tagsMap[tag];
        }
    }
}

std::string GenreRecommenderUser::getNextPopular(std::vector<std::string>& prevTags) {
    long bestTagNum=0;
    std::string bestNextTag;
    long currNum;
    std::string currTag;
    for (std::pair<std::string, long> p : tagsMap){
        currTag = p.first;
        currNum = p.second;
        if ((!(std::find(prevTags.begin(), prevTags.end(), currTag) != prevTags.end()))&&
            ((currNum > bestTagNum) || (currNum == bestTagNum && currTag.compare(bestNextTag) < 0))){
            bestNextTag = currTag;
            bestTagNum = currNum;
        }
    }
    return bestNextTag;
}


