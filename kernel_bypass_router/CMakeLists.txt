cmake_minimum_required(VERSION 3.24)
project(HyperNetAegis_Router VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Aggressive optimizations for core routing logic
add_compile_options(-O3 -march=native -funroll-loops -fno-exceptions -fno-rtti)

# 1. Compile the eBPF/XDP Object (Conceptual compilation via Clang)
# In a real environment, this is compiled separately via clang -target bpf
add_custom_target(ebpf_filter ALL
    COMMAND clang -O2 -target bpf -c ${CMAKE_CURRENT_SOURCE_DIR}/src/ebpf_packet_filter.c -o ${CMAKE_CURRENT_BINARY_DIR}/ebpf_packet_filter.o
    COMMENT "Compiling eBPF XDP Packet Filter..."
)

# 2. Main C++ Router Executable
set(ROUTER_SOURCES
    src/DPDK_MemoryPool.cpp
    src/DeterministicRouter.cpp
)

add_executable(hypernet_router ${ROUTER_SOURCES})

# Include directories for lock-free headers
target_include_directories(hypernet_router PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Threading linkage for core pinning
find_package(Threads REQUIRED)
target_link_libraries(hypernet_router PRIVATE Threads::Threads)
