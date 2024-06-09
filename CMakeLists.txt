cmake_minimum_required(VERSION 3.19)

# 设置项目名称和版本
project(SpriteServer VERSION 1.0.0 LANGUAGES CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 20)

message("Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message("Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

# 默认构建类型为 Release，允许通过命令行或 GUI 配置
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# 编译器选项配置
if(MSVC)
    add_compile_options(/utf-8)  # 应用于所有配置
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Od /Zi)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2)
    endif()
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-O0 -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3)
    endif()
endif()

# 查找必要的依赖包
find_package(libhv CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-libmysql REQUIRED)
find_package(Poco REQUIRED COMPONENTS Data DataMySQL)
find_package(fmt CONFIG REQUIRED)

set(SPRITE_CORE_PATH 
    "libs/SpriteCore"
    CACHE STRING "Path to SpriteCore"
)

add_subdirectory(
    ${SPRITE_CORE_PATH}
    SpriteCore
)

add_executable(
    ${PROJECT_NAME}
    src/SpriteClient/AuthenticatedState.cpp
    src/SpriteClient/UnauthenticatedState.cpp
    src/SpriteClient/State.cpp
    src/SpriteClient/Context.cpp
    src/main.cpp
)

# 设置库的公共头文件目录
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${SPRITE_CORE_PATH}/include
)

# 链接库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    spdlog::spdlog_header_only
    SpriteCore::lib
    nlohmann_json::nlohmann_json
    unofficial::libmysql::libmysql
    Poco::Data Poco::DataMySQL
    fmt::fmt-header-only
)

# 平台特定的库链接
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE hv)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE hv_static)
endif()