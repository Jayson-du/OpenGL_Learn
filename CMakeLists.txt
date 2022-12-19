# 最低版本
cmake_minimum_required(VERSION 3.4)

# 工程名称
project (computer_graphics C CXX)

# 编译类型, 如果没有指定则默认为Debug
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Debug")
endif()

# 设置C++版本
set(CMAKE_CXX_STANDARD 11)
# 如果CMAKE_CXX_STANDARD_REQUIRED设置为True，则必须使用CMAKE_CXX_STANDARD指定的版本，
# 如果CMAKE_CXX_STANDARD_REQUIRED设置为OFF则CMAKE_CXX_STANDARD指定版本的为首选版本，如果没有会使用上一版本
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 开启项目分组
set_property (GLOBAL PROPERTY USE_FOLDERS ON)

# ----设置第三方库的查找的*.cmake路径----
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/;${CMAKE_SOURCE_DIR}")

set (GLFW_LIB_PATH    "${CMAKE_CURRENT_SOURCE_DIR}/resource/library/")
set (GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resource/include/")
# 查找第三方依赖库, GLFW为库名,
# [COMPONENTS]: 后面的组件必须都要找到, 一个没有找到即为失败
# glfw3: 组件名(动态/静态库实际名称)
# [REQUIRED]:   如果找不到立刻终止
find_package (GLFW COMPONENTS glfw3 REQUIRED)
# -----------------------------------

# 设置公共头文件位置
include_directories (
                     "${GLFW_INCLUDE_DIR}"
                     "${CMAKE_CURRENT_SOURCE_DIR}/glad/include"
                    )

# 通过./cmake/FindGLFW.cmake中的设置, 判断是否找到了库文件, 如果找到了则设置库链接路径, 否则直接终止
if (GLFW_PACK_LIB)
    # 添加依赖库名称
    link_libraries(glfw3)
    # 添加依赖库所在路径
    link_directories("${GLFW_LIB_PATH}")
else (GLFW_PACK_LIB)
    message(FATAL_ERROR "glfw3 library not found")
endif(GLFW_PACK_LIB)

#----------------------------
# 编译主CMakeLists下的各个子项目
#----------------------------

# glad项目
add_subdirectory(glad)

# practice1项目
add_subdirectory(practice1)

# practice2项目
add_subdirectory(practice2)


set (STARTUP "practice2")
# Windows下是指当前启动项目
set_property (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${STARTUP})