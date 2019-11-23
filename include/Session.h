#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    std::vector<Watchable*> getContent() const;
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    User& get_active_user();
    std::unordered_map<std::string,User*> getUserMap() const;
    void add_to_user_map(User* user, std::string name);
    void change_user(User* user);
    void erase_user(std::string name);
    std::string secondInput() const;
    std::string thirdInput() const;
    std::vector<BaseAction*>& getActionsLog();
    void clear();
private:
    Watchable* last;
    std::vector<std::string> possibleActions;
    std::string command;
    std::string second;
    std::string third;
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;

};
#endif