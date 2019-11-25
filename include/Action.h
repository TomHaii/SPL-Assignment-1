#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
protected:
    std::string toStringHelper(const std::string&) const;
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    virtual void act(Session&);
    virtual std::string toString() const;
};

class ChangeActiveUser : public BaseAction {
public:
    virtual void act(Session&);
    virtual std::string toString() const;
};

class DeleteUser : public BaseAction {
public:
    virtual void act(Session &);
    virtual std::string toString() const;
};


class DuplicateUser : public BaseAction {
public:
    virtual void act(Session &);
    virtual std::string toString() const;
};

class PrintContentList : public BaseAction {
public:
    virtual void act (Session&);
    virtual std::string toString() const;
};

class PrintWatchHistory : public BaseAction {
public:
    virtual void act (Session&);
    virtual std::string toString() const;
};


class Watch : public BaseAction {
public:
    virtual void act(Session&);
    virtual std::string toString() const;
};


class PrintActionsLog : public BaseAction {
public:
    virtual void act(Session&);
    virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
    virtual void act(Session&);
    virtual std::string toString() const;
};
#endif
