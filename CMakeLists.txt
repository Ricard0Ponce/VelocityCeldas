cmake_minimum_required(VERSION 3.29)
project(VelocityCeldas)

set(CMAKE_CXX_STANDARD 26)

add_executable(VelocityCeldas main.cpp
        Atomo.cpp
        Atomo.h
        Sim.cpp
        Sim.h
        Celda.cpp
        Celda.h)
