//
// Created by tomu@wincs.cs.bgu.ac.il on 19/11/2019.
//

#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"

Watchable::Watchable(long _id, int _length, const std::vector<std::string> &_tags):
        id(_id),length(_length), tags(_tags)  {

}

Watchable::~Watchable() {

}

Movie::Movie(long _id, const std::string& _name, int _length, const std::vector<std::string>& _tags):
        Watchable(_id, _length, _tags), name(_name)
{}

Episode::Episode(long _id, const std::string& _seriesName,int _length, int _season, int _episode ,const std::vector<std::string>& _tags):
        Watchable(_id, _length, _tags), season(_season), episode(_episode), seriesName(_seriesName), nextEpisodeId(0){}

Watchable* Movie::getNextWatchable(Session &session) const {
    return session.get_active_user().getRecommendation(session);
}

std::vector<std::string> Watchable:: getTags() const{
    return tags;
}

Watchable* Episode::getNextWatchable(Session & _sess) const {
    if( nextEpisodeId != -1){
        return _sess.getContent().at(nextEpisodeId);
    }
    return _sess.get_active_user().getRecommendation(_sess);
}

std::string Episode::getName() const {
    return seriesName;
}
std::string Movie::getName() const {
    return name;
}

long Watchable::getId() const {
    return id;
}

std::string Movie::toString() const {
    std::string output = std::to_string(getId()) +". " + name + " " + std::to_string(getLength())+ " minutes [";
    for (std::string s : getTags()){
        output += s + ", ";
    }
    output = output.substr(0, output.length()-2);
    output += "]";
    return output;
}

std::string Episode::toString() const {
    std::string output =  std::to_string(getId()) +". " + toStringHistory() + " ";
    output += std::to_string(getLength()) + " minutes [";
    for (std::string s : getTags()) {
        output += s + ", ";
    }
    output = output.substr(0, output.length() - 2);
    output += "]";
    return output;
}

std::string Movie::toStringHistory() const {
    return name;
}

std::string Episode::toStringHistory() const {
    std::string ep = "E"; std::string sea = "S";
    if (season < 10){
        sea += "0" + std::to_string(season);
    }
    else{
        sea += "" + std::to_string(season);;
    }
    if (episode < 10) {
        ep += "0" + std::to_string(episode);;
    }
    else{
        ep += "" +  std::to_string(episode);
    }
    std::string output = seriesName +" "+ sea + ep;
    return output;
}

void Episode::setNextEpisode(long _id) {
    nextEpisodeId = _id;
}

int Watchable::getLength() const{
    return length;
}