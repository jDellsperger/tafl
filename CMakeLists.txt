# good style

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(Tafl)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

#set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# find sources
file(GLOB SRCS
    ./src/*.cpp
)
source_group("src" FILES ${SRCS})

# find headers (let them show up in the IDEs)
file(GLOB HDRS
    ./inc/*.h
)
source_group("inc" FILES ${HDRS})

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})