#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"
#include <algorithm>
#include <utility>
#include <vector>
#include <cstring>


User::User(const std::string  &_name) :history({}), name(_name),recommendedAlgorithm("len"){
}

std::string User::getName() const {
    return name;
}

std::string User::getRecommendedAlgorithm() const {
    return recommendedAlgorithm;
}

void User::setRecommendedAlgorithm(std::string s)  {
    recommendedAlgorithm = std::move(s);
}

std::vector<Watchable*> User::get_history() const{
    return history;
}

void User::set_history(std::vector<Watchable*> _history) {
    history = std::move(_history);
}

void User::addToHistory(Watchable* w) {
    history.push_back(w);
}

User::~User() {

}

LengthRecommenderUser::LengthRecommenderUser(const std::string& _name): User(_name), average(0) {
    setRecommendedAlgorithm("len");
}

Watchable* LengthRecommenderUser::getRecommendation(Session& s){
    Watchable* returnedShow = nullptr;
    long bestDifference = std::numeric_limits<long>::max();
    for(Watchable*& cont: s.getContent()){
        long difference = std::abs(average - cont->getLength());
        if((!(std::find(history.begin(), history.end(), cont) != history.end())) && difference < bestDifference) {
            bestDifference = difference;
            returnedShow = cont;
        }
    }
    return returnedShow;
}

void LengthRecommenderUser::addToHistory(Watchable* w) {
    long sum = average*(long)history.size();
    sum += w->getLength();
    average = sum/(long)(history.size()+1);
    history.push_back(w);
}

LengthRecommenderUser *LengthRecommenderUser::clone() const {
    return new LengthRecommenderUser(*this);
}


RerunRecommenderUser::RerunRecommenderUser(const std::string& _name):User(_name), lastRecommendation(-1){
    setRecommendedAlgorithm("rer");
}

Watchable* RerunRecommenderUser::getRecommendation(Session& s) {
    lastRecommendation = (lastRecommendation +1 ) % (long)(history.size());
    return history.at(lastRecommendation);
}


RerunRecommenderUser *RerunRecommenderUser::clone() const {
    return new RerunRecommenderUser(*this);
}


GenreRecommenderUser::GenreRecommenderUser(const std::string& _name):User(_name) {
    setRecommendedAlgorithm("gen");
}

Watchable* GenreRecommenderUser::getRecommendation(Session& s) {
    std::unordered_map<std::string,long> map;
    std::pair<std::string, long> popularTag;
    for(Watchable* cont: history){
        for (std::string &tag : cont->getTags()) {
            map[tag]++;
            if (map.count(tag) == 0){
                map[tag] = 1;
            }
            if ((map[tag] > popularTag.second) || (map[tag] == popularTag.second && tag.compare(popularTag.first)<0)) {
                popularTag.first = tag;
                popularTag.second = map[tag];
            }
        }
    }
    std::vector<std::string> prevTags;
    std::string wantedTag = popularTag.first;
    Watchable *next = nullptr;
    long i = 1;
    while (!wantedTag.empty()) {
        i++;
        for (Watchable *w : s.getContent()) {
            std::vector<std::string> tags = w->getTags();
            if ((std::find(tags.begin(), tags.end(), wantedTag) != tags.end() &&
                 (!(std::find(history.begin(), history.end(), *&w) != history.end())))) {
                return w;
            }
        }
        prevTags.push_back(wantedTag);
        long bestTagNum=0;
        std::string bestNextTag;
        long currNum;
        std::string currTag;
        for (std::pair<std::string, long> p : map){
            currTag = p.first;
            currNum = p.second;
            std::string tmpCurrentTag;
            for(char c: currTag){
                tmpCurrentTag += tolower(c);
            }
            std::string tmpBestNextTag;
            for(char c: bestNextTag){
                tmpBestNextTag += tolower(c);
            }
            if ((!(std::find(prevTags.begin(), prevTags.end(), currTag) != prevTags.end()))&&
                ((currNum > bestTagNum) || (currNum == bestTagNum && tmpCurrentTag.compare(tmpBestNextTag) < 0))){
                bestNextTag = currTag;
                bestTagNum = currNum;
            }
        }
        wantedTag = bestNextTag;
    }
    return next;

}

GenreRecommenderUser *GenreRecommenderUser::clone() const {
    return new GenreRecommenderUser(*this);
}

void RerunRecommenderUser::addToHistory(Watchable * w) {
    history.push_back(w);
}

void GenreRecommenderUser::addToHistory(Watchable * w) {
    history.push_back(w);
}





