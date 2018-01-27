cmake_minimum_required(VERSION 3.9)
project(AST)

set(CMAKE_CXX_STANDARD 11)

add_executable(AST main.cpp Tool.h Client/Client.cpp Client/Client.h Tool.cpp Tester/Tester.h Tester/Tester.cpp Server/Server.cpp Server/Server.h)