cmake_minimum_required(VERSION 3.21)
project(todo-app)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(todo-app main.cpp todo.h)
target_link_libraries(todo-app
    SQLiteCpp
    sqlite3
    pthread
    dl
)