cmake_minimum_required(VERSION 3.19)

project("CPl W01"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_library("TetrisPP" STATIC
    "lib/Game.cpp"
    "lib/Print.cpp"
)

target_include_directories("TetrisPP" PUBLIC
    "lib"
)

add_executable("tetrispp"
    "app/Main.cpp"
)

target_link_libraries("tetrispp"
    "TetrisPP"
)
