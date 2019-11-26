#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    std::vector<std::string> getTags() const;
    long getId() const;
    //destructor
    virtual ~Watchable();
    //copy constructor
    //copy assignment operator
    //move constructor
    //move assignment operator
    virtual int getLength() const;
    virtual std::string toString() const = 0;
    virtual std::string toStringHistory() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual std::string getName() const = 0;

private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    Movie(const Movie& otherMovie);
    virtual std::string toString() const;
    virtual std::string toStringHistory() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual std::string getName() const;

private:
    std::string name;
};


class Episode : public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    Episode(const Episode& otherMovie);
    virtual std::string toString() const;
    virtual std::string toStringHistory() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual void setNextEpisode(long);
    virtual std::string getName() const;
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;

};

#endif

