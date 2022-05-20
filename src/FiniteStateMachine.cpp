#include "FSM/FiniteStateMachine.h"
#include <iostream>

FiniteStateMachine::FiniteStateMachine(std::vector<BaseState*> states, 
    CmdPanel *cmdPanel, size_t cmdChannel, double dt)
    :_states(states), _cmdChannel(cmdChannel), _cmdPanel(cmdPanel){
    
    _currentState = states.at(0);
    std::cout << "FSM start from " << _currentState->getStateName() << std::endl;
    _currentState->enter();
    _nextState = _currentState;

    _running = true;
    _mode = FSMRunMode::NORMAL;

    _runThread = new LoopFunc("FSM", dt, boost::bind(&FiniteStateMachine::_staticRun, this));
    _runThread->start();
}

FiniteStateMachine::~FiniteStateMachine(){
    _running = false;
    delete _runThread;
}

void FiniteStateMachine::_run(){
    if(_mode == FSMRunMode::NORMAL){
        _currentState->run();
        _nextStateEnum = _currentState->checkChange((_cmdPanel->getState(_cmdChannel)));
        if(!_currentState->isState(_nextStateEnum)){
            _mode = FSMRunMode::CHANGE;
            for(int i(0); i<_states.size(); ++i){
                if(_states.at(i)->isState(_nextStateEnum)){
                    _nextState = _states.at(i);
                    std::cout << "Switched from " << _currentState->getStateName()
                        << " to " << _nextState->getStateName() << std::endl;
                    break;
                }
            }
        }
    }
    else if(_mode == FSMRunMode::CHANGE){
        _currentState->exit();
        _currentState = _nextState;
        _currentState->enter();
        _mode = FSMRunMode::NORMAL;
        _currentState->run();
    }

}

void* FiniteStateMachine::_staticRun(void* obj){
    if(reinterpret_cast<FiniteStateMachine*>(obj)->_running){
        reinterpret_cast<FiniteStateMachine*>(obj)->_run();
    }
}