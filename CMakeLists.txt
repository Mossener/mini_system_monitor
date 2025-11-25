set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/extern/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")
cmake_minimum_required(VERSION 3.21)
project(SystemEventMonitor LANGUAGES CXX)

# 强制使用现代 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用 vcpkg manifest（自动加载 vcpkg.json）
set(VCPKG_FEATURE_FLAGS "manifests")



# 源码扫描
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(SystemEventMonitor ${SOURCES})

target_include_directories(SystemEventMonitor
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# 引入 vcpkg 包
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(SystemEventMonitor
    PRIVATE
        fmt::fmt
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

