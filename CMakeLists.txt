 
cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Change add_executable
project(server LANGUAGES CXX)
add_executable(
  server 
  server.cpp
  utils.h utils.cpp 
  clientManager.h clientManager.cpp
)
target_link_libraries(server pthread)


project(client LANGUAGES CXX)
add_executable(
  client 
  client.cpp
  utils.h utils.cpp 
  clientManager.h clientManager.cpp
)
target_link_libraries(client pthread)
