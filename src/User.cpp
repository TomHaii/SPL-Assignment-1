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
    history = _history;
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

Watchable* GenreRecommenderUser::getRecommendation(Session& s){
    std::string wantedTag = mostPopularTag.first;
    std::string str;
    while (wantedTag != str) {
        for (Watchable *w : s.getContent()) {
            std::vector<std::string> tags = w->getTags();
            if (std::find(tags.begin(), tags.end(), wantedTag) != tags.end()) {
                if (!(std::find(history.begin(), history.end(), w) != history.end())) {
                    return  &*w;
                }
            }
        }
        str = getNextPopular(wantedTag);
    }
    return nullptr;
}


void GenreRecommenderUser::increaseTag(std::string &tag) {
    bool found = false;
    if(!popularTags.empty()) {
        for (std::pair<std::string, long> &p: popularTags) {
            if (p.first == tag) {
                p.second += 1;
                found = true;
            }
        }
    }
    if(!found){
        std::pair<std::string, long> p (tag,1);
        popularTags.push_back(p);
    }
    if (tag == mostPopularTag.first) {
        mostPopularTag.second += 1;
    }
}


void GenreRecommenderUser::addToHistory(Watchable* w) {
    history.push_back(w);
    std::pair<std::string, long> returned;
    for (std::string tag : w->getTags()) {
        increaseTag(tag);
    }
    std::string tag = mostPopularTag.first;
    long num = mostPopularTag.second;
    for (std::pair<std::string, long> &p : popularTags) {
        if ((p.second > num) || (p.second == num && p.first.compare(tag)<0)){
            num = p.second;
            tag = p.first;
        }
    }
    mostPopularTag.first = tag;
    mostPopularTag.second = num;
}

std::string GenreRecommenderUser::getNextPopular(std::string curr) {
    std::string next = curr;
    long currNum=0;
    for (std::pair<std::string, long>& p : popularTags){
        if (p.first == curr){
            currNum =p.second;
        }
    }
    for (std::pair<std::string, long>& p : popularTags){
        if (p.first != curr && p.second == currNum && p.first.compare(next)<0){
            next = p.first;
        }
        else if (p.first != curr && p.second >currNum){
            next = p.first;
            currNum = p.second;
        }
    }
    return next;
}


