cmake_minimum_required(VERSION 3.24)
project(HyperNetAegis_Consensus VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization for distributed networking logic
if(MSVC)
    add_compile_options(/O2 /arch:AVX2 /W4)
else()
    add_compile_options(-O3 -march=native -Wall -Wextra -pthread)
endif()

# Source definitions
set(CONSENSUS_SOURCES
    src/VectorClock.cpp
    src/DoubleSpendDetector.cpp
    src/RaftCoordinator.cpp
    src/GlobalStateReplicator.cpp
)

add_executable(hypernet_consensus ${CONSENSUS_SOURCES})

# Include directories
target_include_directories(hypernet_consensus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Threads REQUIRED)
target_link_libraries(hypernet_consensus PRIVATE Threads::Threads)
