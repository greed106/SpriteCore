cmake_minimum_required(VERSION 3.19)

# 设置项目名称和版本
project(SpriteCore VERSION 1.0.0 LANGUAGES CXX)

# 指定C++标准
set(CMAKE_CXX_STANDARD 17)

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


# 库名称通过变量管理，方便未来可能的重命名
set(LIBRARY_NAME "lib" CACHE STRING "Library name for the project")

# 创建库
add_library(${LIBRARY_NAME}
    src/Type.cpp
    src/Sprite.cpp
    src/Stirps.cpp
)

# 为库创建别名
add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

# 设置库的公共头文件目录
target_include_directories(${LIBRARY_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)


# 链接库
target_link_libraries(${LIBRARY_NAME}
    PRIVATE
    spdlog::spdlog_header_only
)

# 平台特定的库链接
if(WIN32)
    target_link_libraries(${LIBRARY_NAME} PRIVATE hv)
elseif(UNIX)
    target_link_libraries(${LIBRARY_NAME} PRIVATE hv_static)
endif()

# 根据配置决定是否构建可执行文件
option(BUILD_EXECUTABLE "Build executable" ON)

if(BUILD_EXECUTABLE)
    add_executable(${PROJECT_NAME} src/main.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARY_NAME})
endif()
