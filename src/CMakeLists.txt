cmake_minimum_required(VERSION 4.0)
project(mtmc CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/cpp-httplib)

if(APPLE)
    find_library(cfnetwork_lib CFNetwork REQUIRED)
    find_library(corefoundation_lib corefoundation REQUIRED)
endif()

set(frameworks ${cfnetwork_lib} ${corefoundation_lib})

find_package(OpenSSL REQUIRED)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/pugixml)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    indexers/Torznab.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE httplib OpenSSL::SSL OpenSSL::Crypto ${frameworks} pugixml
)
