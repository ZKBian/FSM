cmake_minimum_required(VERSION 3.12)
project(fsm)

include_directories(
    include
    "/home/$ENV{USER}/bianLib"
    # "/home/bian/testField"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread")

add_executable(fsm 
    test.cpp 
    src/FiniteStateMachine.cpp
    /home/$ENV{USER}/bianLib/time/src/AbsoluteTimer.cpp
    /home/$ENV{USER}/bianLib/multiThread/src/Loop.cpp
    /home/$ENV{USER}/bianLib/CmdPanel/src/CmdPanel.cpp
    /home/$ENV{USER}/bianLib/Keyboard/src/Keyboard.cpp
)