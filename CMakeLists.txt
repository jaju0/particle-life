cmake_minimum_required(VERSION 3.22)

#set(CMAKE_BUILD_TYPE "Debug")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(particle_life)

find_package(Boost 1.82 REQUIRED)

add_executable(${PROJECT_NAME}
    src/ProgramOptions.cpp
    src/ThreadPool.cpp
    src/Particle.cpp
    src/QuadTree.cpp
    src/ParticleSpawners/ParticleSpawnerRandomUniform.cpp
    src/ParticleSpawners/ParticleSpawnerRandomNormal.cpp
    src/App.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    boost_program_options
)