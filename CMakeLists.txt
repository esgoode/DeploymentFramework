cmake_minimum_required(VERSION 2.8)
project('assignment-3')

enable_testing()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable (mShell MasterServerMain.cpp MasterServer.cpp MasterServer.h PracticalSocket.cpp PracticalSocket.h utilities.cpp utilities.h)

add_executable (cShell ServerClientMain.cpp ServerClient.cpp ServerClient.h PracticalSocket.cpp PracticalSocket.h utilities.cpp utilities.h app.cpp app.h)

