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
    virtual ~BaseAction();
    virtual BaseAction* clone() const = 0;
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
protected:
    std::string toStringHelper(const std::string&) const;
    void complete();
    std::string getErrorMsg() const;
    void error(const std::string& errorMsg);
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    virtual CreateUser* clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class ChangeActiveUser : public BaseAction {
public:
    virtual ChangeActiveUser* clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class DeleteUser : public BaseAction {
public:
    virtual DeleteUser* clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};


class DuplicateUser : public BaseAction {
public:
    virtual DuplicateUser* clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class PrintContentList : public BaseAction {
public:
    virtual PrintContentList* clone() const;
    virtual void act (Session& sess);
    virtual std::string toString() const;
};

class PrintWatchHistory : public BaseAction {
public:
    virtual PrintWatchHistory* clone() const;
    virtual void act (Session& sess);
    virtual std::string toString() const;
};


class Watch : public BaseAction {
public:
    virtual Watch* clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};


class PrintActionsLog : public BaseAction {
public:
    virtual PrintActionsLog* clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
    virtual Exit* clone() const;
    virtual void act(Session& sess);
    virtual std::string toString() const;
};
#endif
