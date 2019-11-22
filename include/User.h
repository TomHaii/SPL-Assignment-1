#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>


class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::string getRecommendedAlgorithm() const;
    void setRecommendedAlgorithm(std::string) ;
    std::vector<Watchable*> get_history() const;
    void addToHistory(Watchable*);
    void set_history(std::vector<Watchable*> _history);
//    void increaseTag(std::string &tag);
    std::vector<std::pair<std::string,long>>& getPopularTags();

protected:
    std::vector<Watchable*> history;
private:
    const std::string name;
    std::string recommendedAlgorithm;
    std::vector<std::pair<std::string, long>> popularTags;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);


private:
    Watchable* lastRecommenedWatchable;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
};

#endif
