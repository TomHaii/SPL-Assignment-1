//
// Created by tomu@wincs.cs.bgu.ac.il on 19/11/2019.
//

#include "../include/Watchable.h"


Watchable::Watchable(long _id, int _length, const std::vector<std::string> &_tags):
        id(_id),length(_length), tags(_tags)  {

}

Watchable::~Watchable() {

}

Movie::Movie(long _id, const std::string& _name, int _length, const std::vector<std::string>& _tags):
        Watchable(_id, _length, _tags), name(_name)
{}

Episode::Episode(long _id, const std::string& _seriesName,int _length, int _season, int _episode ,const std::vector<std::string>& _tags):
        Watchable(_id, _length, _tags), season(_season), episode(_episode), seriesName(_seriesName) {}

Watchable* Movie::getNextWatchable(Session &session) const {
    return nullptr;
}


Watchable* Episode::getNextWatchable(Session &) const {
    return nullptr;
}

std::string Movie::toString() const {
    return "blabla";
}


std::string Episode::toString() const {
    return "fsd";
}

int Watchable::getLength() const{
return length;
}