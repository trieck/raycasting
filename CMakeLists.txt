CMAKE_MINIMUM_REQUIRED(VERSION 3.3)
PROJECT(raycasting)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
SET(SOURCE_FILES
        instantcg.h
        instantcg.cpp
        raycaster_flat.cpp
        )

ADD_EXECUTABLE(raycasting ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(raycasting SDL2)
