cmake_minimum_required(VERSION 4.0)
project(mtmc CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    Downloader.cpp
    Indexer.cpp
    indexers/Torznab.h
)
