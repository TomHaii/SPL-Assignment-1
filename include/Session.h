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
    //destructor
    ~Session();
    //copy constructor
    Session(const Session& other);
    //copy assignment operator
    Session& operator=(const Session& other);
    //move constructor
    Session(const Session&& other);
    //move assignment operator
    Session&operator=(const Session&& other);
    void fillDataStructures(const std::vector<Watchable*> &_content, const std::vector<BaseAction*> &_actionLog, const std::unordered_map<std::string, User*> &_userMap);
    void start();
    User& getActiveUser() const;
    std::vector<BaseAction*> getActionsLog() const;
    std::unordered_map<std::string,User*> getUserMap() const;
    void add_to_user_map(User* user, std::string &name);
    void change_user(User* user);
    void erase_user(std::string name);
    std::string secondInput() const;
    std::string thirdInput() const;
    void clear();
private:
    std::string command;
    std::string second;
    std::string third;
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;

};
#endif