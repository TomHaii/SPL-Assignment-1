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
    virtual User* clone() const = 0;
    std::string getName() const;
    std::string getRecommendedAlgorithm() const;
    std::vector<Watchable*> get_history() const;
    void set_history(std::vector<Watchable*> _history);
    virtual void addToHistory(Watchable*);

protected:
    void setRecommendedAlgorithm(std::string);
    std::vector<Watchable*> history;
private:
    std::string name;
    std::string recommendedAlgorithm;


};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual LengthRecommenderUser* clone() const;
    virtual Watchable* getRecommendation(Session& s);
    virtual void addToHistory(Watchable*);
private:
    long average;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual RerunRecommenderUser* clone() const;
    virtual Watchable* getRecommendation(Session& s);
private:
    long lastRecommendation;
};

class GenreRecommenderUser : public User {
public:
    virtual GenreRecommenderUser* clone() const;
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);

private:

};

#endif
