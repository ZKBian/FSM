#ifndef BIANLIB_BASESTATE_H
#define BIANLIB_BASESTATE_H

#include <string>

class BaseState{
public:
    BaseState(int stateNameEnum, std::string stateNameString)
        : _stateNameEnum(stateNameEnum), _stateNameString(stateNameString){}
    virtual ~BaseState(){};

    virtual void enter() = 0;
    virtual void run() = 0;
    virtual void exit() = 0;
    virtual int checkChange(int cmd) = 0;

    bool isState(int stateEnum){
        if(_stateNameEnum == stateEnum){
            return true;
        }else{
            return false;
        }
    }
    std::string getStateName(){return _stateNameString;}
protected:
    int _stateNameEnum;
    std::string _stateNameString;
};

#endif