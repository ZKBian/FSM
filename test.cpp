#include "FiniteStateMachine.h"
#include "CmdPanel/Keyboard.h"
#include <iostream>

enum class FSMStates{
    NONE,
    NEXT,
    S01,
    S02,
    S03,
    NEXT01,
    NEXT02,
    NEXT03
};

class State01 : public BaseState{
public:
    State01():BaseState((int)FSMStates::S01, "State01"){}

    void enter(){std::cout << _stateNameString << " enter" << std::endl;}
    void run(){
        std::cout << _stateNameString << " run" << std::endl;
        // usleep(1000000);
    }
    void exit(){std::cout << _stateNameString << " exit" << std::endl;}
    int checkChange(int cmd){
        if(cmd == (int)FSMStates::NEXT){
            return (int)FSMStates::NEXT01;
        }
        switch (cmd){
        case (int)FSMStates::S02:
            return cmd;
        default:
            return _stateNameEnum;
        }
    }
};

class State02 : public BaseState{
public:
    State02(CmdPanel *cmd):BaseState((int)FSMStates::S02, "State02"), _cmd(cmd){}

    void enter(){
        std::cout << _stateNameString << " enter" << std::endl;
        _cmd->setValue(0.5, 1);
    }
    void run(){
        // std::cout << _stateNameString << " run" << std::endl;
        std::cout << "values: " << _cmd->getValues().at(0) << ", " << _cmd->getValues().at(1) << std::endl;
    }
    void exit(){std::cout << _stateNameString << " exit" << std::endl;}
    int checkChange(int cmd){
        switch (cmd){
        case (int)FSMStates::S01:
        case (int)FSMStates::S03:
            return cmd;
        default:
            return _stateNameEnum;
        }
    }
private:
    CmdPanel *_cmd;
};

class State03 : public BaseState{
public:
    State03():BaseState((int)FSMStates::S03, "State03"){}

    void enter(){std::cout << _stateNameString << " enter" << std::endl;}
    void run(){std::cout << _stateNameString << " run" << std::endl;}
    void exit(){std::cout << _stateNameString << " exit" << std::endl;}
    int checkChange(int cmd){
        // switch (cmd){
        // case (int)FSMStates::S01:
        //     return cmd;
        // default:
        //     return _stateNameEnum;
        // }
        return (int)FSMStates::S01;
    }
};

class StateNext : public BaseState{
public:
    StateNext(int selfEnum, std::string selfName, int nextEnum)
        :BaseState(selfEnum, selfName), _nextEnum(nextEnum){}

    void enter(){std::cout << _stateNameString << " enter" << std::endl;}
    void run(){std::cout << _stateNameString << " run" << std::endl;}
    void exit(){std::cout << _stateNameString << " exit" << std::endl;}
    int checkChange(int cmd){
        if(cmd == (int)FSMStates::NEXT){
            return _nextEnum;
        }
        switch (cmd){
        case (int)FSMStates::S01:
            return cmd;
        default:
            return _stateNameEnum;
        }
    }
private:
    int _nextEnum;
};

int main(){
    EmptyAction empty((int)FSMStates::NONE);
    std::vector<KeyAction*> events;
    events.push_back(new StateAction("1", (int)FSMStates::S01));
    events.push_back(new StateAction("2", (int)FSMStates::S02));
    events.push_back(new StateAction("3", (int)FSMStates::S03));
    events.push_back(new StateAction("4", (int)FSMStates::NEXT));

    events.push_back(new KeyValueAction("q", "a", "z", 1.0, -3.0, 2.0));
    events.push_back(new KeyValueAction("up", "down", 1.0));

    Keyboard key(events, empty, 2);

    std::vector<BaseState*> states;
    states.push_back(new State01());
    states.push_back(new State02(&key));
    states.push_back(new State03());
    states.push_back(new StateNext((int)FSMStates::NEXT01, "next01", (int)FSMStates::NEXT02));
    states.push_back(new StateNext((int)FSMStates::NEXT02, "next02", (int)FSMStates::NEXT03));
    states.push_back(new StateNext((int)FSMStates::NEXT03, "next03", (int)FSMStates::S01));


    FiniteStateMachine fsm01(states, &key, 0, 0.001);
    FiniteStateMachine fsm02(states, &key, 1, 0.5);

    while(true){
        usleep(10000);
    }

    return 0;
}