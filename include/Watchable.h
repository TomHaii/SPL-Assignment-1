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

    virtual int getLength() const;
    virtual Watchable* clone() const = 0;
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
    virtual Movie* clone() const;
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
    virtual std::string toString() const;
    virtual Episode* clone() const;
    virtual std::string toStringHistory() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual void setNextEpisode(long);
    virtual std::string getName() const;
private:
    int season;
    int episode;
    std::string seriesName;
    long nextEpisodeId;

};

#endif

