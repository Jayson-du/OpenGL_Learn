# 最低版本
cmake_minimum_required(VERSION 2.6)

# 工程名称
project (computer_graphics C CXX)

# 编译类型
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Debug")
endif()

# 开启项目分组
set_property (GLOBAL PROPERTY USE_FOLDERS ON)

# 编译主CMakeLists下的各个子项目

# glfw项目
add_subdirectory(glfw)

# practice1项目
add_subdirectory(practice1)